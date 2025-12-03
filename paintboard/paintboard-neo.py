import time
import uuid
import logging
import asyncio
import struct
from typing import Tuple, Optional

import yaml
import colorlog
import requests
import websockets
from PIL import Image


def get_logger(level=logging.INFO):
    """生成带颜色的日志输出"""
    # 创建logger对象
    log = logging.getLogger()
    log.setLevel(level)
    # 创建控制台日志处理器
    console_handler = logging.StreamHandler()
    console_handler.setLevel(level)
    # 定义颜色输出格式
    color_formatter = colorlog.ColoredFormatter(
        fmt="%(log_color)s[%(levelname)s][%(asctime)s] %(message)s",
        datefmt="%a %d %b %Y %H:%M:%S",
    )
    # 将颜色输出格式添加到控制台日志处理器
    console_handler.setFormatter(color_formatter)
    # 移除默认的handler
    for handler in log.handlers:
        log.removeHandler(handler)
    # 将控制台日志处理器添加到logger对象
    log.addHandler(console_handler)
    return log


logger = get_logger()

BASE_DOMIN = "paintboard.luogu.me"
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

MAX_CLIENTS = 3
MAX_CONNECTIONS = 3
MAX_PACKETS_PER_SECOND_PER_CONNECTION = 128
SEND_INTERVAL = 1.0 / MAX_PACKETS_PER_SECOND_PER_CONNECTION
BATCH_SEND_INTERVAL = 0.05

work_queue = asyncio.Queue()


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
        logger.warning("获取PaintKey时出现错误: %s", error)
    assert (
        res.get("token") is not None
    ), f"PaintKey获取失败(uid: {uid}): {res.get("errorType")}"
    return res.get("token")


class PaintClient:
    """Paint client"""

    def __init__(self, token: str):
        self.token = token
        self.ws: Optional[websockets.ClientConnection] = None

    async def worker(self):
        pass  # TODO

    async def maintainer(self):
        pass  # TODO


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
                await work_queue.put(((x, y), pixels[x, y]))

    # Step 4: access PaintKey
    tokens = [access_token(*account) for account in accounts]

    # Step 5: create tasks and run
    clients = [PaintClient(token) for token in tokens]
    workers = [asyncio.create_task(client.worker()) for client in clients]
    maintainers = [asyncio.create_task(client.maintainer()) for client in clients]
    asyncio.gather(*workers, *maintainers)

    # Step 6: clean
    for client in clients:
        if client.ws is not None:
            await client.ws.close()


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
