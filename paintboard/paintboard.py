"""
Origin: https://www.luogu.me/paste/t07d9xob
"""

import time
import uuid
import logging
import asyncio
import struct
import collections
from typing import Optional, Tuple, List

import yaml
import colorlog
import requests
import websockets
import numpy as np
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

# -------------------------- 常量定义 --------------------------
BASE_DOMIN = "paintboard.luogu.me"
GET_TOKEN_URL = f"https://{BASE_DOMIN}/api/auth/gettoken"
WS_URL = f"wss://{BASE_DOMIN}/api/paintboard/ws"

# 操作码定义
OP_S2C_HEARTBEAT = 0xFC  # 服务端心跳请求
OP_S2C_PAINT_MSG = 0xFA  # 他人绘画消息
OP_S2C_PAINT_RESULT = 0xFF  # 绘画结果响应

OP_C2S_HEARTBEAT = 0xFB  # 客户端心跳响应
OP_C2S_PAINT = 0xFE  # 绘画操作

# 绘画结果状态码
PAINT_STATUS = {
    0xEF: "成功",
    0xEE: "冷却中",
    0xED: "Token无效",
    0xEC: "请求格式错误",
    0xEB: "无权限",
    0xEA: "服务器错误",
}

# 全局限制（根据 README 调整）
MAX_CLIENTS = 3
MAX_CONNECTIONS = 3
MAX_PACKETS_PER_SECOND_PER_CONNECTION = 128  # 每连接每秒最多128个包
SEND_INTERVAL = 1.0 / MAX_PACKETS_PER_SECOND_PER_CONNECTION

# 粘包设置
BATCH_SEND_INTERVAL = 0.05

# 任务队列
task_queue = asyncio.Queue()
maintain_queue = asyncio.Queue()

client_semaphore = asyncio.Semaphore(MAX_CLIENTS)
connection_semaphore = asyncio.Semaphore(MAX_CONNECTIONS)


# -------------------------- 工具函数 -------------------------- #
def uint_to_bytes(value: int, byte_length: int) -> bytes:
    """将整数转换为指定长度的小端序字节流"""
    if byte_length == 2:
        return struct.pack("<H", value)  # Uint16
    elif byte_length == 3:
        return bytes(
            [value & 0xFF, (value >> 8) & 0xFF, (value >> 16) & 0xFF]
        )  # Uint24
    elif byte_length == 4:
        return struct.pack("<I", value)  # Uint32
    else:
        raise ValueError(f"不支持的字节长度: {byte_length}")


def uuid_to_bytes(token: str) -> bytes:
    """将UUID格式的Token转换为16字节流"""
    return uuid.UUID(token).bytes


# -------------------------- Token获取 -------------------------- #
def get_paint_token(uid: int, access_key: str) -> Optional[str]:
    """获取绘画所需的Token"""
    try:
        response = requests.post(
            GET_TOKEN_URL,
            json={"uid": uid, "access_key": access_key},
            headers={"Content-Type": "application/json"},
            timeout=10,
        )
        response.raise_for_status()
        result = response.json()

        if "data" in result:
            data = result["data"]
            if data.get("errorType"):
                logger.error("获取Token失败: %s", data["errorType"])
                return None
            return data.get("token")
        return None
    except Exception as error:
        logger.warning("获取Token时出错: %s", error)
        raise error


# -------------------------- 图像处理 -------------------------- #
def read_image(image_path: str, target_size: Tuple[int, int]) -> Optional[np.array]:
    """
    读取图片并保持原始尺寸，返回像素数据
    :param image_path: 图片路径
    :param target_size: 目标尺寸
    :return: 像素数据 np.array[height][width]
    """
    try:
        with Image.open(image_path) as img:
            # 获取尺寸
            width, height = target_size
            logger.info(
                "读取图片：尺寸(%s, %s)，像素数%s",
                *target_size,
                width * height,
            )

            # 校验尺寸
            if not (0 < width <= 1000 and 0 < height <= 600):
                logger.warning("图片尺寸(%s, %s)超出画板限制(1000, 600)", *target_size)
                return None

            # 转换为RGB模式（去除Alpha透明通道，避免像素颜色异常）
            logger.info("尺寸转换: (%s, %s) -> (%s, %s)", *img.size, *target_size)
            # TODO: use original size if config "size" is None
            # TODO: proportional scaling if weight or height is -1
            img = img.convert("RGB").resize(target_size)

            return np.asarray(img)
    except FileNotFoundError:
        logger.error("图片文件未找到：%s", image_path)
        return None
    except Exception as error:
        logger.error("读取图片时出错: %s", error)
        raise error


# ------------------------- 结构体定义 ------------------------- #
PaintEvent = collections.namedtuple("PaintEvent", ["x", "y", "r", "g", "b"])
PaintResult = collections.namedtuple("PaintResult", ["paint_id", "status"])


# ---------------------- WebSocket客户端 ---------------------- #
class PaintboardClient:
    """绘画客户端"""

    def __init__(self, uid: int, access_key: str, token: str):
        self.access_key = access_key
        self.uid = uid
        self.token = token
        self.ws: Optional[websockets.client.ClientConnection] = None
        self.paint_id = hash(self) % 4294967296  # 绘画识别码（自增，确保唯一性）
        self.paint_queue = bytearray()  # 绘画包队列（处理粘包）
        self.send_lock = asyncio.Lock()  # 发送锁
        self.recv_lock = asyncio.Lock()  # 接收锁
        self.packet_counter = 0  # 包计数器
        self.last_send_time = 0  # 上次发送时间
        self.paint_events = {}
        self.paint_events_lock = asyncio.Lock()

    async def shutdown(self):
        """关闭客户端"""
        connection_semaphore.release()
        if self.connection_check():
            await self.ws.close()
        client_semaphore.release()

    async def connect(self) -> bool:
        """建立WebSocket连接"""
        if self.connection_check():
            return True
        await connection_semaphore.acquire()
        try:
            self.ws = await websockets.connect(WS_URL)
            logger.info("WebSocket连接成功: %s", self.ws.remote_address)
            # 启动粘包发送任务
            asyncio.create_task(self._send_queued_paint())
            return True
        except websockets.exceptions.WebSocketException as e:
            logger.warning("WebSocket连接失败: %s", e)
            return False
        except Exception as error:
            logger.error("连接异常: %s", error)
            raise error

    def connection_check(self) -> bool:
        """检查连接状况"""
        return self.ws and self.ws.state == websockets.protocol.State.OPEN

    async def _send_queued_paint(self):
        """定时发送队列中的绘画请求（处理粘包逻辑）"""
        while True:
            queue_size = len(self.paint_queue)
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
                            if len(self.paint_queue) > 0:  # 再次检查队列
                                await self.ws.send(self.paint_queue)
                                sent_packets = len(self.paint_queue) // 29
                                self.packet_counter += sent_packets
                                logger.debug(
                                    "发送粘包数据: %s 字节，包含 %s 个绘画指令",
                                    len(self.paint_queue),
                                    sent_packets,
                                )
                                self.paint_queue.clear()  # 发送后清空队列
                                self.last_send_time = time.time()
                    else:
                        # 数据包太少，等待积累更多数据
                        await asyncio.sleep(0.01)
                except websockets.exceptions.ConnectionClosed:
                    break
                except Exception as error:
                    logger.warning("发送绘画请求失败: %s", error)
                    raise error
            await asyncio.sleep(BATCH_SEND_INTERVAL)  # 批量发送间隔

    async def _handle_messages(self):
        """持续处理服务端消息（心跳、绘画结果等）"""
        while True:
            try:
                async with self.recv_lock:
                    message = await self.ws.recv()
                if isinstance(message, bytes):
                    await self._response_message(message)
            except websockets.exceptions.ConnectionClosed as error:
                logger.warning(
                    "WebSocket连接已关闭(%s %s)", error.rcvd.code, error.rcvd.reason
                )
                break
            except Exception as error:
                logger.warning("处理消息异常: %s", error)
                raise error

    async def _response_message(self, data: bytes):
        """响应服务端二进制消息"""
        for res in self._parse_message(data):
            if res is None:
                try:
                    await self.ws.send(OP_C2S_HEARTBEAT.to_bytes())
                except websockets.exceptions.ConnectionClosedError as error:
                    logger.warning(
                        "WebSocket连接已关闭(%s %s)", error.rcvd.code, error.rcvd.reason
                    )
                    break
            elif isinstance(res, PaintResult):
                status_msg = PAINT_STATUS.get(res.status, f"未知状态码{res.status}")
                logger.debug(
                    "绘画结果[AccessKey: %s, ID: %s]: %s",
                    self.access_key,
                    res.paint_id,
                    status_msg,
                )
                try:
                    async with self.paint_events_lock:
                        event = self.paint_events.pop(res.paint_id)
                    if res.status == 0xEE:  # 冷却状态
                        await task_queue.put(event)
                        logger.info("检测到冷却状态，增加等待时间")
                        await asyncio.sleep(10)
                except KeyError:
                    pass
            elif isinstance(res, PaintEvent):
                # 将他人修改的像素信息加入维护队列
                await maintain_queue.put(res)

    def _parse_message(self, data: bytes) -> List[Optional[PaintEvent | PaintResult]]:
        res = []
        offset = 0
        total_len = len(data)

        while offset < total_len:
            op = data[offset]
            offset += 1

            # 处理心跳请求（立即回复Pong）
            if op == OP_S2C_HEARTBEAT:
                res.append(None)
            # 处理绘画结果（反馈成功/冷却等状态）
            elif op == OP_S2C_PAINT_RESULT:
                if offset + 5 > total_len:
                    break
                paint_id = struct.unpack("<I", data[offset : offset + 4])[0]
                status = data[offset + 4]
                res.append(PaintResult(paint_id, status))
                offset += 5
            # 处理他人绘画消息（用于维护模式）
            elif op == OP_S2C_PAINT_MSG:
                if offset + 6 > total_len:
                    break
                x = struct.unpack("<H", data[offset : offset + 2])[0]
                y = struct.unpack("<H", data[offset + 2 : offset + 4])[0]
                r = data[offset + 4]
                g = data[offset + 5]
                b = data[offset + 6]
                res.append(PaintEvent(x, y, r, g, b))
                offset += 7
            else:
                logger.warning("收到未知操作码: %s", op)
                break
        return res

    async def paint_pixel(self, event: PaintEvent) -> Optional[int]:
        """发送单个像素的绘画请求"""
        x, y, r, g, b = event

        # 校验坐标范围 ([0, 1000), [0, 600))
        if not (0 <= x < 1000 and 0 <= y < 600):
            logger.warning("坐标(%s, %s)超出画板范围，跳过", x, y)
            return None

        # 校验颜色值 [0, 255]
        if not (0 <= r <= 255 and 0 <= g <= 255 and 0 <= b <= 255):
            logger.warning("颜色值(%s, %s, %s)无效，跳过", r, g, b)
            return None

        # 生成唯一绘画识别码
        self.paint_id = (self.paint_id + 1) % 4294967296  # Uint32最大值
        current_id = self.paint_id

        # 构建绘画数据包（共29字节：8字节头部 + 21字节附加信息）
        packet = bytearray()
        # 头部：操作码(1) + x(2) + y(2) + R(1) + G(1) + B(1)
        packet.append(OP_C2S_PAINT)
        packet.extend(uint_to_bytes(x, 2))
        packet.extend(uint_to_bytes(y, 2))
        packet.extend([r, g, b])
        # 附加信息：UID(3) + Token(16) + 绘画ID(4)
        packet.extend(uint_to_bytes(self.uid, 3))
        packet.extend(uuid_to_bytes(self.token))
        packet.extend(uint_to_bytes(current_id, 4))

        # 加入发送队列（粘包处理）
        await asyncio.sleep(BATCH_SEND_INTERVAL)
        async with self.paint_events_lock:
            self.paint_events.update({current_id: event})
        async with self.send_lock:
            self.paint_queue.extend(packet)

        return current_id

    async def worker(self):
        """工作协程：从任务队列获取任务并执行"""
        if not await self.connect():
            return

        try:
            # 启动消息处理任务
            asyncio.create_task(self._handle_messages())

            # 等待连接稳定
            await asyncio.sleep(1)

            # 处理绘制任务
            while self.connection_check():
                try:
                    # 从任务队列获取绘制任务
                    task = await asyncio.wait_for(task_queue.get(), timeout=1.0)
                    await self.paint_pixel(task)
                    task_queue.task_done()
                except asyncio.TimeoutError:
                    # 超时继续循环
                    continue
                except Exception as error:
                    logger.error("处理绘制任务异常: %s", error)
                    raise error
        except Exception as error:
            raise error
        finally:
            # 清理资源
            await self.shutdown()
            logger.info("工作客户端已关闭，总共发送了 %s 个数据包", self.packet_counter)

    async def maintainer(self, pixels: np.array, start_x: int, start_y: int):
        """维护协程：从维护队列获取任务并执行"""
        if not await self.connect():
            return

        try:
            # 启动消息处理任务
            asyncio.create_task(self._handle_messages())

            # 等待连接稳定
            await asyncio.sleep(1)

            width = len(pixels[0])
            height = len(pixels)

            logger.info(
                "维护者开始监听区域 (%s, %s) 到 (%s, %s)",
                start_x,
                start_y,
                start_x + width - 1,
                start_y + height - 1,
            )

            # 处理维护任务
            while self.connection_check():
                try:
                    # 从维护队列获取任务
                    task = await asyncio.wait_for(maintain_queue.get(), timeout=1.0)
                    # 检查是否在监控区域内
                    x = task.x - start_x
                    y = task.y - start_y
                    if 0 <= x < width and 0 <= y < height:
                        # 获取该位置应该的颜色
                        r, g, b = pixels[y][x]

                        # 检查颜色是否被修改
                        if r != task.r or g != task.g or b != task.b:
                            logger.debug(
                                "检测到像素(%s, %s)被修改: (%s, %s, %s) -> (%s, %s, %s)",
                                r,
                                g,
                                b,
                                *task,
                            )

                            # 重新绘制正确的颜色
                            await self.paint_pixel(PaintEvent(task.x, task.y, r, g, b))
                    maintain_queue.task_done()
                except asyncio.TimeoutError:
                    # 超时继续循环
                    continue
                except Exception as error:
                    logger.error("处理维护任务异常: %s", error)
                    raise error
        except Exception as error:
            raise error
        finally:
            # 清理资源
            await self.shutdown()
            logger.info("维护客户端已关闭，总共发送了 %s 个数据包", self.packet_counter)


async def fill_task_queue(
    pixels: np.array, start_x: int, start_y: int, ignore_white: bool = False
):
    """将图像像素填充到任务队列中"""
    height = len(pixels)
    width = len(pixels[0])
    if height == 0:
        logger.warning("像素数据为空")
        return

    # 校验绘制范围是否超出画板
    if start_x < 0 or start_y < 0 or start_x + width > 1000 or start_y + height > 600:
        logger.warning(
            "绘制范围超出画板: 起始(%s, %s) + 尺寸(%s, %s)",
            start_x,
            start_y,
            width,
            height,
        )
        return

    logger.info(
        "填充任务队列：起始(%s, %s)，尺寸(%s, %s)",
        start_x,
        start_y,
        width,
        height,
    )

    # 将所有像素任务加入队列
    for y in range(height):
        for x in range(width):
            r, g, b = pixels[y][x]
            if ignore_white and (r, g, b) == (255, 255, 255):
                continue
            paint_event = PaintEvent(start_x + x, start_y + y, r, g, b)
            await task_queue.put(paint_event)

    logger.info("任务队列填充完成，共添加 %s 个任务", task_queue.qsize())


async def main():
    """
    使用任务队列进行多账号绘图
    """
    # TODO: passing arguments via command line
    try:
        with open("config.yml", "r", encoding="utf-8") as file:
            config = yaml.load(file, yaml.Loader)
        assert isinstance(config, dict)
    except FileNotFoundError as error:
        logger.error("找不到配置文件：%s", error.filename)
        return

    settings = config.get("settings")
    accounts = config.get("accounts")
    if not any((settings, accounts)):
        logger.error("配置项缺失")
        return

    start_x, start_y = settings.get("pos")
    # 1. 获取账号列表和token
    tokens = [get_paint_token(*account) for account in accounts]
    if len(tokens) == 0:
        logger.error("Token获取失败")
        return
    if not any(tokens):
        logger.warning("部分Token获取失败")

    # 2. 读取图片
    pixels = read_image(settings.get("pic"), settings.get("size"))
    if pixels is None:
        logger.error("图片处理失败")
        return

    # 3. 填充任务队列
    await fill_task_queue(pixels, start_x, start_y, True)

    # 4. 启动客户端
    clients: PaintboardClient = []
    tasks: asyncio.Task = []
    for token, account in zip(tokens, accounts):
        await client_semaphore.acquire()
        client = PaintboardClient(*account, token)
        # worker
        task = asyncio.create_task(client.worker())
        clients.append(client)
        tasks.append(task)
        # maintainer
        task = asyncio.create_task(client.maintainer(pixels, start_x, start_y))
        clients.append(client)
        tasks.append(task)

    # 5. 等待所有任务完成
    try:
        await asyncio.gather(*tasks)
    finally:
        for client in clients:
            await client.shutdown()


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        logger.info("程序被用户中断")
    except Exception as error:
        logger.error("程序运行异常: %s", error)
        raise error
