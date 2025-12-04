"""
New Paintboard Client
"""

import time
import asyncio
import itertools
from uuid import UUID
from typing import Optional, Tuple

import yaml
import colorlog
import requests
import websockets
import numpy as np
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
        datefmt="%Y %b %a %d %H:%M:%S",
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

# URLs
BASE_DOMIN = "paintboard.luogu.me"
GET_BOARD_URL = f"https://{BASE_DOMIN}/api/paintboard/getboard"
GET_TOKEN_URL = f"https://{BASE_DOMIN}/api/auth/gettoken"
WS_URL = f"wss://{BASE_DOMIN}/api/paintboard/ws"

# 打包、解包操作
BYTE_ORDER = "little"
# 头部: 操作码(1) + x(2) + y(2) + R(1) + G(1) + B(1)
PAINTEVENT_SHAPE = (1, 2, 2, 1, 1, 1)
# 附加信息: UID(3) + Token(16) + 绘画ID(4)
ADDITION_SHAPE = (3, 16, 4)
# 绘画结果: 操作码(1) + 识别码(4) + 状态码(1)
RESULT_SHAPE = (1, 4, 1)
# Ping: 操作码(1)
HEARTBEAT_SHAPE = (1,)

# 操作码
OP_S2C_HEARTBEAT = 0xFC
OP_S2C_PAINT_MSG = 0xFA
OP_S2C_PAINT_RESULT = 0xFF

OP_C2S_HEARTBEAT = 0xFB
OP_C2S_PAINT = 0xFE

OP_S2C_MAPPINGS = {
    OP_S2C_HEARTBEAT: HEARTBEAT_SHAPE,
    OP_S2C_PAINT_MSG: PAINTEVENT_SHAPE,
    OP_S2C_PAINT_RESULT: RESULT_SHAPE,
}

# 绘画结果状态码
PAINT_STATUS = {
    0xEF: "成功",
    0xEE: "冷却中",
    0xED: "Token无效",
    0xEC: "请求格式错误",
    0xEB: "无权限",
    0xEA: "服务器错误",
}

# 连接设置
MAX_CONNECTIONS = 3
MAX_PACKETS_PER_SECOND_PER_CONNECTION = 128
SEND_INTERVAL = 1.0 / MAX_PACKETS_PER_SECOND_PER_CONNECTION
BATCH_SEND_INTERVAL = 0.2

# 全局资源
pixel_queue = asyncio.Queue()

connection_semaphore = asyncio.Semaphore(MAX_CONNECTIONS)


# ------------------- HTTP APIs ------------------- #
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


def access_token(uid: int, key: str) -> UUID:
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
    token = data.get("token")
    return UUID(token)


# ----------------- WebSocket APIs ----------------- #
class PaintClient:
    """Paint client"""

    def __init__(
        self,
        account: Tuple[int, str],
        pixels,
        area: Tuple[Tuple[int, int], Tuple[int, int]],
    ):
        self.uid, self.key = account
        self.token: UUID = access_token(*account)
        self.pixels = pixels
        self.area = area
        self.running = False
        self.ws: Optional[websockets.ClientConnection] = None
        self.packet_count = 0
        self.send_queue = bytearray()
        self.send_lock = asyncio.Lock()
        self.paint_history = {}
        self.history_lock = asyncio.Lock()
        self.last_send_time = 0

    async def connect(self):
        """Connect to server"""
        await connection_semaphore.acquire()
        self.ws = await websockets.connect(WS_URL)
        await asyncio.sleep(1)
        logger.info("连接到服务器[uid: %s]", self.uid)

    async def disconnect(self):
        """Disconnect from server"""
        logger.info("关闭连接[uid: %s]", self.uid)
        if self.ws is not None:
            await self.ws.close()
        connection_semaphore.release()

    async def pack(self, pixel: Tuple[Tuple[int, int], Tuple[int, int, int]]) -> bytes:
        """Pack C2S message"""

        def packer(msg: Tuple, shape: Tuple) -> bytes:
            return b"".join(
                num if isinstance(num, bytes) else num.to_bytes(size, BYTE_ORDER)
                for num, size in zip(msg, shape)
            )

        # 构建绘画数据包（共29字节：8字节头部 + 21字节附加信息）
        self.packet_count = (self.packet_count + 1) % 4294967296  # uint32 max
        ((x, y), (r, g, b)) = pixel
        header = (OP_C2S_PAINT, x, y, r, g, b)
        addition = (self.uid, self.token.bytes, self.packet_count)
        async with self.history_lock:
            self.paint_history.update({self.packet_count: pixel})
        return packer(header, PAINTEVENT_SHAPE) + packer(addition, ADDITION_SHAPE)

    def unpack(self, data: bytes) -> Tuple[Tuple[int, ...], ...]:
        """Unpack S2C message"""

        def unpacker(msg: bytes, shape: Tuple) -> Tuple[Tuple[bytes, ...], bytes]:
            res = np.split(bytearray(msg), tuple(itertools.accumulate(shape)))
            return (
                tuple(block.tobytes() for block in res[: len(shape)]),
                res[-1].tobytes(),
            )

        blocks, remaining = unpacker(data, OP_S2C_MAPPINGS.get(data[0]))
        res = (tuple(int.from_bytes(block) for block in blocks),)
        while len(remaining):
            blocks, remaining = unpacker(remaining, OP_S2C_MAPPINGS.get(remaining[0]))
            res += (tuple(int.from_bytes(block) for block in blocks),)
        return res

    async def _recv(self) -> Optional[bytes]:
        try:
            return await self.ws.recv()
        except websockets.exceptions.ConnectionClosedOK as ok:
            logger.info("连接正常关闭[uid: %s]: %s %s", self.uid, ok.rcvd.code, ok.rcvd.reason)
        except websockets.exceptions.ConnectionClosedError as error:
            logger.warning("连接非正常关闭[uid: %s]: %s %s", self.uid, error.rcvd.code, error.rcvd.reason)
        return None

    async def _send(self, message: bytes) -> bool:
        await asyncio.sleep(SEND_INTERVAL)
        try:
            await self.ws.send(message)
            return True
        except websockets.exceptions.ConnectionClosedOK as ok:
            logger.info("连接正常关闭[uid: %s]: %s %s", self.uid, ok.rcvd.code, ok.rcvd.reason)
        except websockets.exceptions.ConnectionClosedError as error:
            logger.warning("连接非正常关闭[uid: %s]: %s %s", self.uid, error.rcvd.code, error.rcvd.reason)
        return False

    async def _heartbeat(self):
        """Keep client alive"""
        logger.info("心跳[uid: %s]", self.uid)
        await self._send(OP_C2S_HEARTBEAT.to_bytes())

    async def _send_queued_paint(self):
        """定时发送队列中的绘画请求（处理粘包逻辑）"""
        logger.info("开始处理发包[uid: %s]", self.uid)
        while self.running:
            queue_size = len(self.send_queue)
            if queue_size > 0:
                try:
                    # 检查是否有数据包进行发送
                    packet_count = queue_size // 29  # 每个包29字节
                    if packet_count >= 1:  # 有包就发送
                        # 控制发送速率（每个连接独立控制）
                        current_time = time.time()
                        elapsed = current_time - self.last_send_time
                        if elapsed < SEND_INTERVAL * packet_count:
                            await asyncio.sleep(SEND_INTERVAL * packet_count - elapsed)

                        # 发送数据
                        async with self.send_lock:
                            if len(self.send_queue) > 0:  # 再次检查队列
                                if not await self._send(self.send_queue):
                                    break
                                sent_packets = len(self.send_queue) // 29
                                logger.debug(
                                    "发送粘包数据: %s 字节，包含 %s 个绘画指令",
                                    len(self.send_queue),
                                    sent_packets,
                                )
                                self.send_queue.clear()  # 发送后清空队列
                                self.last_send_time = time.time()
                    else:
                        # 数据包太少，等待积累更多数据
                        await asyncio.sleep(0.01)
                except websockets.exceptions.ConnectionClosed:
                    self.running = False
                    break
                except Exception as error:
                    logger.warning("发送绘画请求失败: %s", error)
                    raise error
            await asyncio.sleep(BATCH_SEND_INTERVAL)  # 批量发送间隔

    async def _paint_pixel(self, pixel: Tuple[Tuple[int, int], Tuple[int, int, int]]):
        """Paint a pixel"""
        ((x, y), (r, g, b)) = pixel
        # 校验坐标范围 ([0, 1000), [0, 600))
        if not (0 <= x < 1000 and 0 <= y < 600):
            logger.warning("坐标(%s, %s)超出画板范围，跳过", x, y)
            return None

        # 校验颜色值 [0, 255]
        if not (0 <= r <= 255 and 0 <= g <= 255 and 0 <= b <= 255):
            logger.warning("颜色值(%s, %s, %s)无效，跳过", r, g, b)
            return None

        await asyncio.sleep(BATCH_SEND_INTERVAL)
        async with self.send_lock:
            self.send_queue.extend(await self.pack(pixel))

    def _check_pixel(self, pixel: Tuple[Tuple[int, int], Tuple[int, int, int]]) -> bool:
        """Check whether a pixel changed"""
        ((xa, ya), (xb, yb)) = self.area
        ((x, y), color) = pixel
        return (
            not (xa <= x <= xb and ya <= y <= yb)
            or self.pixels[x - xa, y - ya] == color
        )

    async def _event_handler(self):
        """Handling events"""
        logger.info("事件监听启动[uid: %s]", self.uid)
        while self.running:
            try:
                recv = await asyncio.wait_for(self._recv(), 3.0)
            except asyncio.TimeoutError:
                continue
            self.running = recv is not None
            for msg in self.unpack(recv):
                opcode, *content = msg
                if opcode == OP_S2C_HEARTBEAT:
                    await self._heartbeat()
                elif opcode == OP_S2C_PAINT_MSG:
                    x, y, r, g, b = content
                    pixel = ((x, y), (r, g, b))
                    if not self._check_pixel(pixel):
                        await pixel_queue.put(pixel)
                elif opcode == OP_S2C_PAINT_RESULT:
                    paint_id, status = content
                    if status == 0xEE:
                        try:
                            async with self.history_lock:
                                await pixel_queue.put(self.paint_history.pop(paint_id))
                        except KeyError:
                            pass
                        logger.info("冷却[uid: %s]", self.uid)
                        await asyncio.sleep(10)

    async def worker(self):
        """Main worker"""
        logger.info("启动客户端[uid: %s]", self.uid)
        self.running = True
        await self.connect()
        asyncio.create_task(self._event_handler())
        asyncio.create_task(self._send_queued_paint())
        while self.running:
            await self._paint_pixel(await pixel_queue.get())
            pixel_queue.task_done()
        await self.disconnect()


async def main():
    """main"""
    # Step 1: read config
    config_path = "config.yml"
    logger.info("读取配置文件: %s", config_path)
    with open(config_path, "r", encoding="utf-8") as file:
        config = yaml.safe_load(file)
    assert isinstance(config, dict), "配置文件格式错误"
    settings = config.get("settings")
    assert settings is not None, "缺少配置项`settings`"
    accounts = config.get("accounts")
    assert accounts is not None and len(accounts) > 0, "未设置绘板账号"

    # Step 2: read image
    logger.info("读取图片: %s", settings.get("pic"))
    assert settings.get("pic") is not None, "未设置图片路径"
    with Image.open(settings.get("pic")) as img_file:
        img = img_file.convert("RGB")
    if settings.get("size") is not None:
        img = img.resize(settings.get("size"))

    # Step 3: init task queue
    logger.info("初始化任务列表")
    pos = settings.get("pos")
    assert pos is not None, "未设置初始位置"
    start_x, start_y = pos
    pixels = img.load()
    width, height = img.size
    for y in range(height):
        for x in range(width):
            if not (settings.get("ignore_white") and pixels[x, y] == (255, 255, 255)):
                await pixel_queue.put(((start_x + x, start_y + y), pixels[x, y]))

    # Step 4: create tasks and run
    logger.info("创建任务并运行")
    area = (pos, (start_x + width - 1, start_y + height - 1))
    clients = [PaintClient(account, pixels, area) for account in accounts]
    workers = [asyncio.create_task(client.worker()) for client in clients]
    await asyncio.gather(*workers)


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        logger.info("中止程序")
    except FileNotFoundError as error:
        logger.error("文件未找到：%s", error.filename)
    except AssertionError as error:
        logger.error("%s", error)
    except Exception as error:
        logger.error("捕获到异常: %s", error)
        raise error
