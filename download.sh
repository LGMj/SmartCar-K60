#!/bin/bash
#
# download.sh - 一键下载程序到 K60 开发板
#
# 用法: ./download.sh [build] [--tool jlink|daplink|openocd]
#   不带参数        - 仅下载（使用上次选中的工具），默认 J-Link
#   build           - 先编译，再下载
#   --tool jlink    - 使用 J-Link 下载（默认）
#   --tool daplink  - 使用 DAPlink/pyocd 下载
#   --tool openocd  - 使用 OpenOCD + CMSIS-DAP 下载
#
# 硬件要求:
#   J-Link 模式:
#     - J-Link 通过 SWD 连接到 K60 开发板
#     - J-Link 驱动已安装
#
#   DAPlink 模式:
#     - DAPlink 固件已烧录到板载调试器
#     - pyocd 已安装: pip install pyocd
#     - 目标芯片: Cortex-M4 (通用目标)
#
#   OpenOCD 模式:
#     - openocd 已安装 (brew install open-ocd 或从源码编译)
#     - CMSIS-DAP 调试器通过 SWD 连接到 K60 开发板
#     - openocd 配置: interface/cmsis-dap.cfg + target/k40.cfg
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SREC_FILE="$SCRIPT_DIR/Output/Debug/Exe/MK60_Blinky.srec"

JLINK_BIN="JLinkExe"
JLINK_DEVICE="MK60DN512XXX10"
JLINK_ARGS="-device $JLINK_DEVICE -if SWD -speed 4000"

# pyocd 目标名称 (K60 基于 Cortex-M4，使用通用目标确保兼容性)
PYOCD_TARGET="Cortex-M4"
PYOCD_BIN="pyocd"

# OpenOCD 配置
OPENOCD_BIN="openocd"
OPENOCD_INTERFACE="interface/cmsis-dap.cfg"
OPENOCD_TARGET="target/k40.cfg"
OPENOCD_SPEED="10000"

TOOL="jlink"

# --- 解析参数 ---
for arg in "$@"; do
    case "$arg" in
        --tool)
            ;;
        --tool=*)
            TOOL="${arg#*=}"
            ;;
        jlink| daplink| openocd)
            TOOL="$arg"
            ;;
    esac
done

# 如果参数中有 --tool xxx，重新解析
if [[ " $@ " =~ " --tool " ]]; then
    for ((i=1; i<=$#; i++)); do
        if [[ "${!i}" == "--tool" ]]; then
            ((j=i+1))
            if [[ $j -le $# ]]; then
                TOOL="${!j}"
            fi
        fi
    done
fi

echo "========================================"
echo "  MK60 Download Script"
echo "  Tool: $(echo $TOOL | tr '[:lower:]' '[:upper:]')"
echo "========================================"
echo ""

# --- 可选：先编译 ---
if [[ "$1" == "build" ]]; then
    echo ">> Step 1/4: Building project..."
    "$SCRIPT_DIR/build.sh"
    echo ""
fi

# --- 检查固件文件 ---
echo ">> Step 2/4: Checking firmware file..."
if [ ! -f "$SREC_FILE" ]; then
    echo "ERROR: Firmware not found: $SREC_FILE"
    echo "Please run './build.sh' first, or use './download.sh build'."
    exit 1
fi
echo "Firmware: $SREC_FILE"
ls -lh "$SREC_FILE"
echo ""

# --- 下载固件 ---
echo ">> Step 3/4: Downloading firmware..."
echo ""

if [ "$TOOL" = "jlink" ]; then
    echo "Connecting to J-Link (device: $JLINK_DEVICE, interface: SWD)..."
    echo "----------------------------------------"

    JLINK_OUTPUT=$($JLINK_BIN $JLINK_ARGS <<EOF
r
halt
loadfile "$SREC_FILE"
r
halt
qc
EOF
)

    echo "$JLINK_OUTPUT"

    if echo "$JLINK_OUTPUT" | grep -q "O.K." && \
       ! echo "$JLINK_OUTPUT" | grep -q "Failed to open file" && \
       ! echo "$JLINK_OUTPUT" | grep -q "Failed to read"; then
        SUCCESS=true
    else
        SUCCESS=false
    fi

    elif [ "$TOOL" = "daplink" ]; then
        echo "Using DAPlink (pyocd, target: $PYOCD_TARGET)..."
        echo "----------------------------------------"

        if ! command -v $PYOCD_BIN &> /dev/null; then
            echo ""
            echo "ERROR: pyocd not found."
            echo "Please install pyocd: pip install pyocd"
            exit 1
        fi

        PYOCD_OUTPUT=$($PYOCD_BIN flash -t $PYOCD_TARGET "$SREC_FILE" 2>&1)
        PYOCD_EXIT=$?

        echo "$PYOCD_OUTPUT"

        if [ $PYOCD_EXIT -eq 0 ] && echo "$PYOCD_OUTPUT" | grep -q -i "success\|complete"; then
            SUCCESS=true
        else
            SUCCESS=false
        fi

    elif [ "$TOOL" = "openocd" ]; then
        echo "Using OpenOCD + CMSIS-DAP..."
        echo "----------------------------------------"

        if ! command -v $OPENOCD_BIN &> /dev/null; then
            echo ""
            echo "ERROR: openocd not found."
            echo "Install: brew install open-ocd"
            exit 1
        fi

        OPENOCD_OUTPUT=$($OPENOCD_BIN \
            -f "$OPENOCD_INTERFACE" \
            -f "$OPENOCD_TARGET" \
            -c "transport select swd" \
            -c "adapter speed $OPENOCD_SPEED" \
            -c "init" \
            -c "kinetis mdm mass_erase 0" \
            -c "halt" \
            -c "flash write_image erase \"$SREC_FILE\"" \
            -c "reset run" \
            -c "shutdown" 2>&1)
        OPENOCD_EXIT=$?

        echo "$OPENOCD_OUTPUT"

        if [ $OPENOCD_EXIT -eq 0 ] && echo "$OPENOCD_OUTPUT" | grep -q "wrote.*bytes"; then
            SUCCESS=true
        else
            SUCCESS=false
        fi
fi

# --- 结果判断 ---
echo ""
echo ">> Step 4/4: Verifying result..."
echo ""

if [ "$SUCCESS" = true ]; then
    echo "========================================"
    echo "  DOWNLOAD SUCCESS"
    echo "========================================"
    echo "Program downloaded and running."
    exit 0
else
    echo "========================================"
    echo "  DOWNLOAD FAILED"
    echo "========================================"

    if [ "$TOOL" = "jlink" ]; then
        echo "Troubleshooting:"
        echo "  1. Is J-Link connected via USB?"
        echo "  2. Is the K60 board powered on?"
        echo "  3. Is the SWD cable properly connected?"
        echo "  4. Is another program holding the J-Link?"
    elif [ "$TOOL" = "daplink" ]; then
        echo "Troubleshooting:"
        echo "  1. Is DAPlink USB drive mounted?"
        echo "  2. Is the K60 board powered on?"
        echo "  3. Is the SWD cable properly connected?"
        echo "  4. Try: pip install --upgrade pyocd"
    elif [ "$TOOL" = "openocd" ]; then
        echo "Troubleshooting:"
        echo "  1. Is CMSIS-DAP debugger connected via USB?"
        echo "  2. Is the K60 board powered on?"
        echo "  3. Is the SWD cable properly connected?"
        echo "  4. If chip is secured, run: openocd -f interface/cmsis-dap.cfg -f target/k40.cfg -c 'transport select swd' -c 'kinetis mdm mass_erase 0' -c shutdown"
        echo "  5. If openocd not found: brew install open-ocd"
    fi
    exit 1
fi
