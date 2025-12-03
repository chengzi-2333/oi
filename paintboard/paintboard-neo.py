import time
import uuid
import asyncio
from typing import Optional, Tuple

import yaml
import colorlog
import requests
import websockets
from PIL import Image


def get_colorful_logger(level=colorlog.INFO):
    """生成带颜色的日志输出"""
    # 创建logger对象
    logger_obj = colorlog.getLogger()
    logger_obj.setLevel(level)
    # 创建控制台日志处理器
    console_handler = colorlog.StreamHandler()
    console_handler.setLevel(level)
    # 定义颜色输出格式
    color_formatter = colorlog.ColoredFormatter(
        fmt="%(log_color)s[%(levelname)s][%(asctime)s] %(message)s",
        datefmt="%a %d %b %Y %H:%M:%S",
    )
    # 将颜色输出格式添加到控制台日志处理器
    console_handler.setFormatter(color_formatter)
    # 移除默认的handler
    for handler in logger_obj.handlers:
        logger_obj.removeHandler(handler)
    # 将控制台日志处理器添加到logger对象
    logger_obj.addHandler(console_handler)
    return logger_obj


logger = get_colorful_logger()

BASE_DOMIN = "paintboard.luogu.me"
GET_BOARD_URL = f"https://{BASE_DOMIN}/api/paintboard/getboard"
GET_TOKEN_URL = f"https://{BASE_DOMIN}/api/auth/gettoken"
WS_URL = f"wss://{BASE_DOMIN}/api/paintboard/ws"

# 操作码定义
OP_S2C_HEARTBEAT = 0xFC
OP_S2C_PAINT_MSG = 0xFA
OP_S2C_PAINT_RESULT = 0xFF

OP_C2S_HEARTBEAT = 0xFB
OP_C2S_PAINT = 0xFE

# 绘画结果状态码
PAINT_STATUS = {
    0xEF: "成功",
    0xEE: "冷却中",
    0xED: "Token无效",
    0xEC: "请求格式错误",
    0xEB: "无权限",
    0xEA: "服务器错误",
}

MAX_CONNECTIONS = 3
MAX_PACKETS_PER_SECOND_PER_CONNECTION = 128
SEND_INTERVAL = 1.0 / MAX_PACKETS_PER_SECOND_PER_CONNECTION
BATCH_SEND_INTERVAL = 0.05

pixel_queue = asyncio.Queue()

connection_semaphore = asyncio.Semaphore(MAX_CONNECTIONS)


def get_board() -> bytes:
    """Get paint board"""
    resp = requests.get(GET_BOARD_URL, timeout=10)
    resp.raise_for_status()
    buffer = resp.content
    pixels = []
    for y in range(600):
        row = []
        for x in range(1000):
            base = y * 1000 * 3 + x * 3
            pixel = (buffer[base], buffer[base + 1], buffer[base + 2])
            row.append(pixel)
        pixels.append(row)
    return pixels


def access_token(uid: int, key: str) -> str:
    """Access PaintKey"""
    resp = requests.post(
        GET_TOKEN_URL,
        json={"uid": uid, "access_key": key},
        headers={"Content-Type": "application/json"},
        timeout=10,
    )
    try:
        resp.raise_for_status()
        res = resp.json()
    except (requests.HTTPError, requests.JSONDecodeError) as error:
        logger.error("获取PaintKey时出现错误: %s", error)
        raise error
    data = res.get("data")
    assert (
        data is not None and data.get("token") is not None
    ), f"PaintKey获取失败(uid: {uid}): {data.get("errorType")}"
    return data.get("token")


class PaintClient:
    """Paint client"""

    def __init__(self, account: Tuple[int, str]):
        self.uid, self.key = account
        self.token = access_token(*account)
        self.ws: Optional[websockets.ClientConnection] = None
        self.packet_count = 0

    async def connect(self):
        """Connect to server"""
        await connection_semaphore.acquire()
        self.ws = await websockets.connect(WS_URL)

    async def disconnect(self):
        """Disconnect from server"""
        if self.ws is not None:
            self.ws.close()
        connection_semaphore.release()

    async def pack(
        self, pixel: Tuple[Tuple[int, int], Tuple[int, int, int]]
    ) -> bytearray:
        """Pack C2S message"""
        # 构建绘画数据包（共29字节：8字节头部 + 21字节附加信息）
        ((x, y), (r, g, b)) = pixel
        self.packet_count = (self.packet_count + 1) % 4294967296  # uint32 max
        packet = bytearray()
        # 头部：操作码(1) + x(2) + y(2) + R(1) + G(1) + B(1)
        packet.append(OP_C2S_PAINT)
        packet.extend(x.to_bytes(2, "little"))
        packet.extend(y.to_bytes(2, "little"))
        packet.extend((r, g, b))
        # 附加信息：UID(3) + Token(16) + 绘画ID(4)
        packet.extend(self.uid.to_bytes(3, "little"))
        packet.extend(uuid.UUID(self.token).bytes)
        packet.extend(self.packet_count.to_bytes(4, "little"))
        return packet

    async def unpack(self):
        """Unpack S2C message"""
        # TODO
        pass

    async def _event_handler(self):
        """Handling events"""
        # TODO
        pass

    async def worker(self):
        """Main worker"""
        asyncio.create_task(self._event_handler())
        # TODO


async def main():
    """main"""
    # Step 1: read config
    with open("config.yml", "r", encoding="utf-8") as file:
        config = yaml.load(file, yaml.Loader)
    assert isinstance(config, dict), "配置文件格式错误"
    settings = config.get("settings")
    assert settings is not None, "缺少配置项`settings`"
    accounts = config.get("accounts")
    assert accounts is not None and len(accounts) > 0, "未设置绘板账号"

    # Step 2: read image
    assert settings.get("pic") is not None, "未设置图片路径"
    with Image.open(settings.get("pic")) as img_file:
        img = img_file.convert("RGB")
    if settings.get("size") is not None:
        img = img.resize(settings.get("size"))

    # Step 3: generate task queue
    pixels = img.load()
    width, height = img.size
    for x in range(width):
        for y in range(height):
            if not (settings.get("ignore_white") and pixels[x, y] == (255, 255, 255)):
                await pixel_queue.put(((x, y), pixels[x, y]))

    # Step 4: create tasks and run
    try:
        clients = [PaintClient(account) for account in accounts]
        workers = [asyncio.create_task(client.worker()) for client in clients]
        asyncio.gather(*workers)

    # Step 5: close connection
    finally:
        for client in clients:
            await client.disconnect()


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        logger.info("中止程序")
    except FileNotFoundError as error:
        logger.error("图片文件未找到：%s", error.filename)
    except AssertionError as error:
        logger.error("%s", error)
    except Exception as error:
        logger.error("捕获到异常: %s", error)
        raise error
