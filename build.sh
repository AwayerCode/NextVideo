#!/bin/bash

# 检查是否存在build目录
if [ ! -d "build" ]; then
    echo "构建目录不存在，创建新的构建目录..."
    mkdir build
    NEED_BUILD=1
else
    # 检查源文件是否有更新
    LATEST_SRC_MOD=$(find src include -type f -printf '%T@ %p\n' | sort -n | tail -1 | cut -f2- -d" ")
    LATEST_SRC_TIME=$(stat -c %Y "$LATEST_SRC_MOD")
    
    if [ -f "build/ffmpeg_test_tool" ]; then
        BINARY_TIME=$(stat -c %Y "build/ffmpeg_test_tool")
        
        if [ $LATEST_SRC_TIME -gt $BINARY_TIME ]; then
            echo "检测到源文件更新，需要重新编译..."
            NEED_BUILD=1
        else
            echo "源文件未更新，直接运行..."
            NEED_BUILD=0
        fi
    else
        echo "可执行文件不存在，需要编译..."
        NEED_BUILD=1
    fi
fi

if [ "$NEED_BUILD" = "1" ]; then
    cd build
    
    # 运行cmake配置
    echo "运行CMake配置..."
    cmake ..
    
    # 编译项目
    echo "编译项目..."
    make -j$(nproc)
    
    # 检查编译结果
    if [ $? -ne 0 ]; then
        echo "构建失败！"
        exit 1
    fi
    
    echo "构建成功！"
else
    cd build
fi

# 在运行程序前添加这些环境变量设置
export QT_QPA_PLATFORM=xcb
export QT_QPA_PLATFORMTHEME=gtk3
export DISPLAY=:0

# 运行程序
echo "正在运行程序..."
./ffmpeg_test_tool 