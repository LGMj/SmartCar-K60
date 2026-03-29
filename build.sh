#!/bin/bash
#
# build.sh - 一键编译 MK60_Blinky 项目
#
# 用法: ./build.sh [clean]
#   不带参数   - 增量编译
#   clean      - 清除输出后重新编译
#

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_FILE="MK60_Blinky.emProject"
SES_BIN="/Applications/SEGGER Embedded Studio for ARM 7.32a/bin"

echo "========================================"
echo "  MK60 PIT LED Project - Build Script"
echo "========================================"
echo ""

if [ ! -f "$PROJECT_FILE" ]; then
    echo "ERROR: Project file not found: $PROJECT_FILE"
    exit 1
fi

if [ ! -x "$SES_BIN/emBuild" ]; then
    echo "ERROR: emBuild not found at: $SES_BIN/emBuild"
    echo "Please check SEGGER Embedded Studio installation path."
    exit 1
fi

BUILD_CMD="\"$SES_BIN/emBuild\" -config Debug \"$PROJECT_FILE\""

if [ "$1" = "clean" ]; then
    echo ">> Cleaning build output..."
    rm -rf "$SCRIPT_DIR/Output/Debug"
    echo ">> Running clean build..."
    BUILD_CMD="$BUILD_CMD -rebuild"
else
    echo ">> Running incremental build..."
fi

echo ""
echo "Command: $BUILD_CMD"
echo "----------------------------------------"

eval $BUILD_CMD
BUILD_STATUS=$?

echo "----------------------------------------"
if [ $BUILD_STATUS -eq 0 ]; then
    echo ""
    echo "========================================"
    echo "  BUILD SUCCESS"
    echo "========================================"
    if [ -f "$SCRIPT_DIR/Output/Debug/Exe/MK60_Blinky.srec" ]; then
        echo "Output: $SCRIPT_DIR/Output/Debug/Exe/MK60_Blinky.srec"
        ls -lh "$SCRIPT_DIR/Output/Debug/Exe/MK60_Blinky.srec"
    fi
    exit 0
else
    echo ""
    echo "========================================"
    echo "  BUILD FAILED (exit code: $BUILD_STATUS)"
    echo "========================================"
    exit $BUILD_STATUS
fi
