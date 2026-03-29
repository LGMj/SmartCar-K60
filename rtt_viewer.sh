#!/bin/bash
#
# rtt_viewer.sh - 通过 J-Link RTT 查看日志
#
# 用法:
#   ./rtt_viewer.sh              # 启动 J-Link RTT Viewer GUI（推荐）
#   ./rtt_viewer.sh build        # 编译 + 下载 + 启动 RTT Viewer
#   ./rtt_viewer.sh cli          # 无头模式（命令行输出到终端）
#
# 前置条件:
#   - J-Link 已通过 USB 连接
#   - K60 开发板已上电
#   - SWD 接口已连接
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
JLINK_DIR="/Applications/SEGGER/JLink_V812a"
DEVICE="MK60DN512XXX10"
IF="SWD"
SPEED="4000"

echo "=========================================="
echo "  J-Link RTT Viewer for K60"
echo "=========================================="
echo "Device: $DEVICE"
echo "Interface: $IF @ ${SPEED}kHz"
echo ""

# 检查 J-Link RTT Viewer 是否存在
if [ ! -f "$JLINK_DIR/JLinkRTTViewer.app/Contents/MacOS/JLinkRTTViewerExe" ]; then
    echo "ERROR: J-Link RTT Viewer not found at:"
    echo "  $JLINK_DIR/JLinkRTTViewer.app/Contents/MacOS/JLinkRTTViewerExe"
    echo ""
    echo "请确认已安装 J-Link 软件包："
    echo "  https://www.segger.com/downloads/jlink/"
    exit 1
fi

MODE="${1:-gui}"

case "$MODE" in
    gui)
        echo ">> Launching J-Link RTT Viewer..."
        echo ""
        echo "在 RTT Viewer 中:"
        echo "  1. 选择 Device: $DEVICE"
        echo "  2. 选择 Interface: $IF"
        echo "  3. Speed: ${SPEED}kHz"
        echo "  4. 点击 OK 连接"
        echo ""
        echo "（同时按住 Cmd+Q 或关闭窗口即可退出）"
        echo ""
        "$JLINK_DIR/JLinkRTTViewer.app/Contents/MacOS/JLinkRTTViewerExe" \
            -Device "$DEVICE" \
            -If "$IF" \
            -Speed "$SPEED" \
            -RTTChannel 0 \
            -Silent \
            &
        wait
        ;;

    cli)
        echo ">> Starting RTT Client (headless mode)..."
        echo "（按 Ctrl+C 退出）"
        echo ""
        # J-Link RTT Client: 自动检测 RTT 并输出到 stdout
        "$JLINK_DIR/JLinkRTTClientExe" \
            -Device "$DEVICE" \
            -If "$IF" \
            -Speed "$SPEED"
        ;;

    build)
        echo ">> Step 1/4: Building project..."
        "$SCRIPT_DIR/build.sh" || exit 1
        echo ""

        echo ">> Step 2/4: Downloading firmware..."
        "$SCRIPT_DIR/download.sh" || exit 1
        echo ""

        echo ">> Step 3/4: Connecting RTT..."
        echo ""
        echo ">> Step 4/4: Launching RTT Viewer..."
        echo ""
        "$JLINK_DIR/JLinkRTTViewer.app/Contents/MacOS/JLinkRTTViewerExe" \
            -Device "$DEVICE" \
            -If "$IF" \
            -Speed "$SPEED" \
            -RTTChannel 0 \
            -Silent \
            &
        wait
        ;;

    *)
        echo "Usage: $0 [gui|cli|build]"
        echo ""
        echo "  gui    - 启动 J-Link RTT Viewer GUI（默认）"
        echo "  cli    - 命令行模式（无 GUI）"
        echo "  build  - 编译 + 下载 + 启动 RTT Viewer"
        exit 1
        ;;
esac
