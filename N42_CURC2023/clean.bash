#!/bin/bash

# 安全清理STM32 CubeMX生成的文件
echo "开始清理所有项目中的CubeMX生成文件..."

# 遍历当前目录下的所有子文件夹
find . -maxdepth 1 -type d ! -name "." | while read -r dir; do
    dir="${dir#./}"  # 移除路径开头的"./"
    echo "处理项目: $dir"
    
    # 删除库文件夹
    rm -rf "${dir}/Drivers" 2>/dev/null
    rm -rf "${dir}/Middlewares" 2>/dev/null
    rm -rf "${dir}/MDK-ARM" 2>/dev/null
    
    # 清理Core/Src目录（保留main.c）
    if [ -d "${dir}/Core/Src" ]; then
        find "${dir}/Core/Src" -type f ! -name "main.c" -delete
    fi
    
    # 清理Core/Inc目录（保留main.h）
    if [ -d "${dir}/Core/Inc" ]; then
        find "${dir}/Core/Inc" -type f ! -name "main.h" -delete
    fi
    
    # 可选：删除CubeMX配置文件
    # find "$dir" -maxdepth 1 -type f \( -name "*.ioc" -o -name "*.mxproject" \) -delete
done

echo "清理完成！"
echo "保留的文件："
echo "  - 所有项目中的 main.c 和 main.h"
echo "  - 项目核心代码文件"
echo "删除的文件："
echo "  - Drivers, Middlewares, MDK-ARM 文件夹"
echo "  - CubeMX自动生成的配置文件"