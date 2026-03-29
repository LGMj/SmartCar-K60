#!/bin/bash
#
# download.sh - 一键下载程序到 K60 开发板
#
# 用法: ./download.sh [build]
#   不带参数   - 仅下载，不编译
#   build      - 先编译，再下载
#
# 硬件要求:
#   - J-Link 通过 SWD 连接到 K60 开发板
#   - J-Link 驱动已安装
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SREC_FILE="$SCRIPT_DIR/Output/Debug/Exe/MK60_Blinky.srec"

JLINK_BIN="JLinkExe"
JLINK_ARGS="-device MK60DN512XXX10 -if SWD -speed 4000"

echo "========================================"
echo "  MK60 Download Script"
echo "========================================"
echo ""

# --- 可选：先编译 ---
if [ "$1" = "build" ]; then
    echo ">> Step 1/3: Building project..."
    "$SCRIPT_DIR/build.sh"
    echo ""
fi

# --- 检查固件文件 ---
echo ">> Step 2/3: Checking firmware file..."
if [ ! -f "$SREC_FILE" ]; then
    echo "ERROR: Firmware not found: $SREC_FILE"
    echo "Please run './build.sh' first, or use './download.sh build'."
    exit 1
fi
echo "Firmware: $SREC_FILE"
ls -lh "$SREC_FILE"
echo ""

# --- 下载固件 ---
echo ">> Step 3/3: Downloading firmware..."
echo ""
echo "Connecting to J-Link (device: MK60DN512XXX10, interface: SWD)..."
echo "----------------------------------------"

# 使用 heredoc 展开变量，将输出捕获用于判断
JLINK_OUTPUT=$($JLINK_BIN $JLINK_ARGS <<EOF
r
halt
loadfile "$SREC_FILE"
r
halt
qc
EOF
)

# 同时输出 J-Link 原始日志
echo "$JLINK_OUTPUT"

# J-Link Commander 本身总是返回 0，loadfile 失败也不会改变退出码
# 因此通过输出内容判断是否真正下载成功
if echo "$JLINK_OUTPUT" | grep -q "O.K." && \
   ! echo "$JLINK_OUTPUT" | grep -q "Failed to open file" && \
   ! echo "$JLINK_OUTPUT" | grep -q "Failed to read"; then
    echo ""
    echo "========================================"
    echo "  DOWNLOAD SUCCESS"
    echo "========================================"
    echo "Program downloaded and running."
    exit 0
else
    echo ""
    echo "========================================"
    echo "  DOWNLOAD FAILED"
    echo "========================================"
    echo "Troubleshooting:"
    echo "  1. Is J-Link connected via USB?"
    echo "  2. Is the K60 board powered on?"
    echo "  3. Is the SWD cable properly connected?"
    echo "  4. Is another program holding the J-Link?"
    exit 1
fi
