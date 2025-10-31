"""
From: https://www.luogu.me/paste/t07d9xob
"""

import time
import uuid
import logging
import asyncio
import struct
import collections
from typing import Optional, Tuple, List, Dict

import yaml
import colorlog
import requests
import websockets
from PIL import Image
from websockets.protocol import State


def get_logger(level=logging.INFO):
    """生成带颜色的日志输出"""
    # 创建logger对象
    logger = logging.getLogger()
    logger.setLevel(level)
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
    for handler in logger.handlers:
        logger.removeHandler(handler)
    # 将控制台日志处理器添加到logger对象
    logger.addHandler(console_handler)
    return logger


logger = get_logger()

# -------------------------- 常量定义 --------------------------
BASE_URL = "paintboard.luogu.me"
GET_TOKEN_URL = f"https://{BASE_URL}/api/auth/gettoken"
WS_URL = f"wss://{BASE_URL}/api/paintboard/ws"

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
MAX_CONNECTIONS = 2
MAX_PACKETS_PER_SECOND_PER_CONNECTION = 128  # 每连接每秒最多128个包
SEND_INTERVAL = 1.0 / MAX_PACKETS_PER_SECOND_PER_CONNECTION

# 粘包设置
BATCH_SEND_INTERVAL = 0.02  # 20ms 发送一次，符合 README 建议

# 任务队列
task_queue = asyncio.Queue()
maintain_queue = asyncio.Queue()


# -------------------------- 工具函数 --------------------------
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


# -------------------------- Token获取 --------------------------
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
    except Exception as e:
        logger.warning("获取Token时出错: %s", e)
        return None


# -------------------------- 图像处理（支持自定义尺寸） --------------------------
def read_image(
    image_path: str, target_size: Tuple[int, int]
) -> Optional[List[List[Dict]]]:
    """
    读取图片并保持原始尺寸，返回像素数据
    :param image_path: 图片路径
    :return: 像素数据
    像素数据格式：[height][width] 二维列表，每个元素为{r, g, b}
    """
    try:
        with Image.open(image_path) as img:
            # 获取图片尺寸
            logger.info(
                "图片原始尺寸: (%s, %s)，目标尺寸：(%s, %s)", *img.size, *target_size
            )
            width, height = target_size

            # 校验尺寸是否符合画板最大限制（1000x600）
            if not (0 < width <= 1000 and 0 < height <= 600):
                logger.warning(
                    "图片尺寸(%s, %s)超出画板限制（最大1000x600）", *target_size
                )
                return None

            # 转换为RGB模式（去除Alpha透明通道，避免像素颜色异常）
            img_resize = img.resize(target_size)
            img_rgb = img_resize.convert("RGB")

            # 按原始尺寸读取像素数据（逐行逐列，确保数据完整性）
            pixels = []
            for y in range(height):
                row = []
                for x in range(width):
                    r, g, b = img_rgb.getpixel((x, y))
                    row.append({"r": r, "g": g, "b": b})
                pixels.append(row)

            logger.info(
                "成功读取图片：尺寸(%s, %s)，总像素%s个",
                *target_size,
                width * height,
            )
            return pixels
    except FileNotFoundError:
        logger.error("图片文件未找到：%s", image_path)
        return None
    except Exception as e:
        logger.error("读取图片时出错: %s", e)
        return None


def chunk_image(pixels: List[List[Dict]], chunk_num: int) -> List[List[List[Dict]]]:
    """将图片分为 chunk_num 个块，大pixels的行数被均分。对于每个chunk，如果总数无法被整除，则最后一个块将多一些"""
    if chunk_num <= 0:
        raise ValueError("chunk_num 必须大于0")

    total_rows = len(pixels)
    if total_rows == 0:
        return []

    # 计算每个块的基础行数和余数
    base_rows_per_chunk = total_rows // chunk_num
    remainder = total_rows % chunk_num

    chunks = []
    start_row = 0

    for i in range(chunk_num):
        # 前remainder个块会多分配一行
        rows_in_this_chunk = base_rows_per_chunk + int(i < remainder)
        end_row = start_row + rows_in_this_chunk

        # 提取当前块的像素数据
        chunk = pixels[start_row:end_row]
        chunks.append(chunk)

        start_row = end_row

    return chunks


# -------------------------- 任务定义 --------------------------
PaintTask = collections.namedtuple("PaintTask", ["x", "y", "r", "g", "b"])
MaintainTask = collections.namedtuple("MaintainTask", ["x", "y", "r", "g", "b"])


# -------------------------- WebSocket客户端 --------------------------
class PaintboardClient:
    def __init__(self, uid: int, token: str, access_key: str):
        self.access_key = access_key
        self.uid = uid
        self.token = token
        self.ws: Optional[websockets.asyncio.client.ClientConnection] = None
        self.paint_id = 0  # 绘画识别码（自增，确保唯一性）
        self.paint_queue = bytearray()  # 绘画包队列（处理粘包）
        self.connected = False  # 连接状态标记
        self.send_lock = asyncio.Lock()  # 发送锁
        self.packet_counter = 0  # 包计数器
        self.last_send_time = 0  # 上次发送时间

    async def connect(self) -> bool:
        """建立WebSocket连接"""
        try:
            self.ws = await websockets.connect(WS_URL)
            self.connected = True
            logger.info("WebSocket连接成功")
            # 启动粘包发送任务
            asyncio.create_task(self._send_queued_paint())
            return True
        except websockets.exceptions.WebSocketException as e:
            logger.warning("WebSocket连接失败: %s", e)
            return False
        except Exception as e:
            logger.error("连接异常: %s", e)
            return False

    async def _send_queued_paint(self):
        """定时发送队列中的绘画请求（处理粘包逻辑）"""
        while self.connected:
            queue_size = len(self.paint_queue)
            if queue_size > 0 and self.ws and self.ws.state == State.OPEN:
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
                except Exception as e:
                    logger.warning("发送绘画请求失败: %s", e)
            await asyncio.sleep(BATCH_SEND_INTERVAL)  # 批量发送间隔

    async def _handle_messages(self):
        """持续处理服务端消息（心跳、绘画结果等）"""
        while self.connected and self.ws and self.ws.state == State.OPEN:
            try:
                message = await self.ws.recv()
                if isinstance(message, bytes):
                    await self._parse_message(message)
            except websockets.exceptions.ConnectionClosed as e:
                logger.warning(
                    "WebSocket连接已关闭：代码 %s，原因：%s", e.code, e.reason
                )
                self.connected = False
                break
            except Exception as e:
                logger.warning("处理消息异常: %s", e)

    async def _parse_message(self, data: bytes):
        """解析服务端二进制消息"""
        offset = 0
        total_len = len(data)

        while offset < total_len:
            op = data[offset]
            offset += 1

            # 处理心跳请求（立即回复Pong）
            if op == OP_S2C_HEARTBEAT:
                if self.ws and self.ws.state == State.OPEN:
                    await self.ws.send(bytes([OP_C2S_HEARTBEAT]))
                continue

            # 处理绘画结果（反馈成功/冷却等状态）
            elif op == OP_S2C_PAINT_RESULT:
                if offset + 5 > total_len:
                    logger.warning("绘画结果数据不完整，跳过")
                    break
                paint_id = struct.unpack("<I", data[offset : offset + 4])[0]
                status = data[offset + 4]
                offset += 5
                status_msg = PAINT_STATUS.get(status, f"未知状态码{status}")
                logger.info(
                    "绘画结果[AccessKey: %s, ID: %s]: %s",
                    self.access_key,
                    paint_id,
                    status_msg,
                )
                # 如果是冷却状态，增加等待时间
                if status == 0xEE:  # 冷却中
                    logger.info("检测到冷却状态，增加等待时间")
                    await asyncio.sleep(10)
                continue

            # 处理他人绘画消息（用于维护模式）
            elif op == OP_S2C_PAINT_MSG:
                if offset + 6 > total_len:
                    break
                x = struct.unpack("<H", data[offset : offset + 2])[0]
                y = struct.unpack("<H", data[offset + 2 : offset + 4])[0]
                r = data[offset + 4]
                g = data[offset + 5]
                b = data[offset + 6]
                offset += 7

                # 将他人修改的像素信息加入维护队列
                await maintain_queue.put(MaintainTask(x, y, r, g, b))
                continue

            # 未知操作码
            else:
                logger.warning("收到未知操作码: %s，跳过", op)
                break

    async def paint_pixel(
        self, x: int, y: int, r: int, g: int, b: int
    ) -> Optional[int]:
        """发送单个像素的绘画请求"""
        # 校验连接状态
        if not self.connected or not self.ws or self.ws.state != State.OPEN:
            logger.warning("未连接到服务器，无法发送绘画请求")
            return None

        # 校验坐标范围（画板0≤x<1000，0≤y<600）
        if not (0 <= x < 1000 and 0 <= y < 600):
            logger.warning("坐标(%s, %s)超出画板范围，跳过", x, y)
            return None

        # 校验颜色值（0-255）
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
            while self.connected:
                try:
                    # 从任务队列获取绘制任务
                    task = await asyncio.wait_for(task_queue.get(), timeout=1.0)
                    if isinstance(task, PaintTask):
                        await self.paint_pixel(*task)
                        # 短暂延时避免发送过快
                        await asyncio.sleep(0.01)
                    task_queue.task_done()
                except asyncio.TimeoutError:
                    # 超时继续循环
                    continue
                except Exception as e:
                    logger.error("处理绘制任务异常: %s", e)

        finally:
            # 清理资源
            self.connected = False
            if self.ws and self.ws.state == State.OPEN:
                await self.ws.close()
            logger.info("客户端已关闭，总共发送了 %s 个数据包", self.packet_counter)

    async def maintainer(
        self,
        pixels_map: Dict[Tuple[int, int], Tuple[int, int, int]],
        start_x: int,
        start_y: int,
        width: int,
        height: int,
    ):
        """维护协程：从维护队列获取任务并执行"""
        if not await self.connect():
            return

        try:
            # 启动消息处理任务
            asyncio.create_task(self._handle_messages())

            # 等待连接稳定
            await asyncio.sleep(1)

            logger.info(
                "维护者开始监听区域 (%s,%s) 到 (%s,%s)",
                start_x,
                start_y,
                start_x + width - 1,
                start_y + height - 1,
            )

            # 处理维护任务
            while self.connected:
                try:
                    # 从维护队列获取任务
                    task = await asyncio.wait_for(maintain_queue.get(), timeout=1.0)
                    if isinstance(task, MaintainTask):
                        # 检查是否在监控区域内
                        if (
                            start_x <= task.x < start_x + width
                            and start_y <= task.y < start_y + height
                        ):

                            # 获取该位置应该的颜色
                            if (task.x, task.y) in pixels_map:
                                expected_r, expected_g, expected_b = pixels_map[
                                    (task.x, task.y)
                                ]

                                # 检查颜色是否被修改
                                if (
                                    expected_r != task.r
                                    or expected_g != task.g
                                    or expected_b != task.b
                                ):

                                    logger.info(
                                        "检测到像素被修改: (%s,%s): (%s, %s, %s) -> (%s, %s, %s)",
                                        *task,
                                        expected_r,
                                        expected_g,
                                        expected_b,
                                    )

                                    # 重新绘制正确的颜色
                                    await self.paint_pixel(
                                        task.x,
                                        task.y,
                                        expected_r,
                                        expected_g,
                                        expected_b,
                                    )
                                    # 短暂延时避免发送过快
                                    await asyncio.sleep(0.1)
                    maintain_queue.task_done()
                except asyncio.TimeoutError:
                    # 超时继续循环
                    continue
                except Exception as e:
                    logger.error("处理维护任务异常: %s", e)

        finally:
            # 清理资源
            self.connected = False
            if self.ws and self.ws.state == State.OPEN:
                await self.ws.close()
            logger.info("维护客户端已关闭，总共发送了 %s 个数据包", self.packet_counter)


async def populate_task_queue(
    pixels: List[List[Dict]], start_x: int, start_y: int, ignore_white: bool = False
):
    """将图像像素填充到任务队列中"""
    target_height = len(pixels)
    if target_height == 0:
        logger.warning("像素数据为空，无法填充任务队列")
        return
    target_width = len(pixels[0])

    # 校验所有行宽度一致
    if any(len(row) != target_width for row in pixels):
        logger.warning("像素数据各行宽度不一致，无效数据")
        return

    # 校验绘制范围是否超出画板
    if (
        start_x < 0
        or start_y < 0
        or start_x + target_width > 1000
        or start_y + target_height > 600
    ):
        logger.warning(
            "绘制范围超出画板！起始(%s, %s)+尺寸(%s, %s) > (1000, 600)",
            start_x,
            start_y,
            target_width,
            target_height,
        )
        return

    logger.info(
        "开始填充任务队列：起始位置(%s, %s)，尺寸(%s, %s)",
        start_x,
        start_y,
        target_width,
        target_height,
    )

    # 将所有像素任务加入队列
    task_count = 0
    for y in range(target_height):
        for x in range(target_width):
            pixel = pixels[y][x]
            if ignore_white and tuple(pixel.values()) == (255, 255, 255):
                continue
            await task_queue.put(PaintTask(start_x + x, start_y + y, *pixel.values()))
            task_count += 1

    logger.info("任务队列填充完成，共添加 %s 个任务", task_count)


def create_pixels_map(
    pixels: List[List[Dict]], start_x: int, start_y: int
) -> Dict[Tuple[int, int], Tuple[int, int, int]]:
    """创建像素位置到颜色的映射"""
    pixels_map = {}
    target_height = len(pixels)
    if target_height == 0:
        return pixels_map
    target_width = len(pixels[0])

    for y in range(target_height):
        for x in range(target_width):
            pixels_map[(start_x + x, start_y + y)] = tuple(pixels[y][x].values())

    return pixels_map


async def main():
    """
    使用任务队列处理多账号绘图
    """
    try:
        with open("config.yml", "r", encoding="utf-8") as file:
            config = yaml.load(file, yaml.Loader)
    except FileNotFoundError as e:
        logger.error("找不到配置文件：%s", e.filename)
        return

    settings = config.get("settings")
    accounts = config.get("accounts")

    START_X, START_Y = settings.get("pos")
    # 1. 获取账号列表和token
    tokens = [get_paint_token(uid, key) for uid, key in accounts]
    if not any(tokens):
        logger.error("获取Token失败，程序退出")
        return

    # 2. 读取图片
    pixels = read_image(settings.get("pic"), settings.get("size"))
    if not pixels:
        logger.error("图片处理失败，程序退出")
        return

    # 创建像素映射用于维护
    pixels_map = create_pixels_map(pixels, START_X, START_Y)

    # 3. 填充任务队列
    await populate_task_queue(pixels, START_X, START_Y, ignore_white=True)

    # 4. 启动工作客户端
    workers = []
    tasks = []

    # 创建工作客户端
    for token, (uid, access_key) in zip(tokens, accounts):
        client = PaintboardClient(uid, token, access_key)
        workers.append(client)

        # 为每个客户端创建工作协程
        task = asyncio.create_task(client.worker())
        tasks.append(task)

    # 5. 启动维护客户端（使用第一个账号）
    maintainer_client = PaintboardClient(accounts[0][0], tokens[0], accounts[0][1])
    maintainer_task = asyncio.create_task(
        maintainer_client.maintainer(
            pixels_map, START_X, START_Y, len(pixels[0]), len(pixels)
        )
    )
    tasks.append(maintainer_task)

    # 6. 等待所有任务完成
    try:
        await asyncio.gather(*tasks)
    except KeyboardInterrupt:
        logger.info("程序被用户中断")
    except Exception as e:
        logger.error("程序运行异常: %s", e)
    finally:
        # 确保所有客户端都正确关闭
        for client in workers:
            if client.connected:
                client.connected = False
                if client.ws and client.ws.state == State.OPEN:
                    await client.ws.close()
        if maintainer_client.connected:
            maintainer_client.connected = False
            if maintainer_client.ws and maintainer_client.ws.state == State.OPEN:
                await maintainer_client.ws.close()
        logger.info("所有客户端已关闭")


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        logger.info("程序被用户中断")
    except Exception as e:
        logger.error("程序运行异常: %s", e)
