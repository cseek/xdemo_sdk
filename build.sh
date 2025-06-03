#!/bin/bash
###
 # @Author: aurson jassimxiong@gmail.com
 # @Date: 2025-06-03 22:24:43
 # @LastEditors: aurson jassimxiong@gmail.com
 # @LastEditTime: 2025-06-04 01:07:29
 # @Description: 
 # Copyright (c) 2025 by Aurson, All Rights Reserved. 
### 
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
project_dir=${script_dir}
build_dir=${project_dir}/build
# 默认参数值
build_type="Release"
platform="X64"
# 需要检查的命令列表
commands=("gcc" "g++" "cmake" "make")
# 存储缺失的命令
missing=()
# 显示帮助信息
usage() {
    echo "用法: $0 [-b <构建类型>] [-p <平台>]"
    echo "选项:"
    echo "  -b, --build-type   指定构建类型 (Debug|Release), 默认: Release"
    echo "  -p, --platform     指定目标平台 (X86|X64|Arm64), 默认: X64"
    echo "  -h, --help         显示此帮助信息"
    echo ""
    echo "示例: $0 -b Release -p X86"
    exit 0
}

# 解析命令行参数
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -b|--build-type)
            if [[ -z "$2" || "$2" == -* ]]; then
                echo "错误: -b 参数需要指定构建类型"
                usage
                exit 1
            fi
            build_type="$2"
            shift
            ;;
        -p|--platform)
            if [[ -z "$2" || "$2" == -* ]]; then
                echo "错误: -p 参数需要指定平台"
                usage
                exit 1
            fi
            platform="$2"
            shift
            ;;
        -h|--help)
            usage
            ;;
        *)
            echo "错误: 未知参数 '$1'"
            usage
            exit 1
            ;;
    esac
    shift
done

# 验证参数值
valid_build_types=("Debug" "Release")
valid_platforms=("X86" "X64" "Arm64")

if [[ ! " ${valid_build_types[@]} " =~ " ${build_type} " ]]; then
    echo "错误: 无效的构建类型 '${build_type}'，有效值: ${valid_build_types[*]}"
    exit 1
fi

if [[ ! " ${valid_platforms[@]} " =~ " ${platform} " ]]; then
    echo "错误: 无效的平台 '${platform}'，有效值: ${valid_platforms[*]}"
    exit 1
fi

# 执行构建
echo "正在构建..."
echo "├─ 构建类型: ${build_type}"
echo "└─ 目标平台: ${platform}"

# 遍历所有命令
for cmd in "${commands[@]}"; do
    if ! command -v "$cmd" &> /dev/null; then
        missing+=("$cmd")
    fi
done

# 检查结果
if [ ${#missing[@]} -eq 0 ]; then
    echo "编译环境正常"
else
    echo "编译环境异常, 以下命令缺失:"
    for miss in "${missing[@]}"; do
        echo "  $miss"
    done
    exit 1
fi

cmake -B ${build_dir} \
      -DENABLE_CPPCHECK=OFF \
      -DCMAKE_TOOLCHAIN_FILE=${project_dir}/cmake/Toolchain${platform}.cmake \
      -DCMAKE_BUILD_TYPE=${build_type} \
      ${project_dir} || exit

cmake --build ${build_dir} --parallel
