#!/bin/bash
###
 # @Author: aurson jassimxiong@gmail.com
 # @Date: 2024-06-03 22:24:43
 # @LastEditors: aurson jassimxiong@gmail.com
 # @LastEditTime: 2024-06-20 01:07:44
 # @Description: 
 # Copyright (c) 2025 by Aurson, All Rights Reserved. 
### 

# 配置路径和默认参数
configure_globals() {
    script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    project_dir="${script_dir}"
    build_dir="${project_dir}/build"
    build_type="Release"
    platform="X64"
    commands=("gcc" "g++" "cmake" "make")
    do_clean=false
}

# 显示帮助信息
show_usage() {
    cat <<EOF
用法: $0 [-b <构建类型>] [-p <平台>] [-c]
选项:
  -b, --build-type   指定构建类型 (Debug|Release), 默认: Release
  -p, --platform     指定目标平台 (X86|X64|Arm64), 默认: X64
  -c, --clean        清理构建目录
  -h, --help         显示此帮助信息

示例: 
  $0 -b Release -p X86
  $0 --clean
EOF
    exit 0
}

# 清理构建目录
clean_build_dir() {
    if [ -d "${build_dir}" ]; then
        echo "正在清理构建目录: ${build_dir}"
        rm -rf "${build_dir}"
        echo "清理完成"
    else
        echo "构建目录不存在，无需清理"
    fi
}

# 参数验证函数
validate_arguments() {
    local valid_build_types=("Debug" "Release")
    local valid_platforms=("X86" "X64" "Arm64")

    if [[ ! " ${valid_build_types[@]} " =~ " ${build_type} " ]]; then
        echo "错误: 无效的构建类型 '${build_type}'，有效值: ${valid_build_types[*]}"
        exit 1
    fi

    if [[ ! " ${valid_platforms[@]} " =~ " ${platform} " ]]; then
        echo "错误: 无效的平台 '${platform}'，有效值: ${valid_platforms[*]}"
        exit 1
    fi
}

# 检查命令是否存在
check_commands() {
    local missing=()
    for cmd in "${commands[@]}"; do
        if ! command -v "$cmd" &> /dev/null; then
            missing+=("$cmd")
        fi
    done

    if [ ${#missing[@]} -ne 0 ]; then
        echo "编译环境异常, 以下命令缺失:"
        for miss in "${missing[@]}"; do
            echo "  $miss"
        done
        return 1
    fi
    echo "编译环境正常"
    return 0
}

# 构建项目
build_project() {
    echo "正在构建..."
    echo "├─ 构建类型: ${build_type}"
    echo "└─ 目标平台: ${platform}"

    cmake -B "${build_dir}" \
          -DENABLE_CPPCHECK=OFF \
          -DCMAKE_TOOLCHAIN_FILE="${project_dir}/cmake/Toolchain${platform}.cmake" \
          -DCMAKE_BUILD_TYPE="${build_type}" \
          "${project_dir}" || return 1

    cmake --build "${build_dir}" --parallel || return 1
}

# 主函数：参数解析和流程控制
main() {
    configure_globals

    # 解析命令行参数
    while [[ "$#" -gt 0 ]]; do
        case "$1" in
            -b|--build-type)
                [[ -z "$2" || "$2" == -* ]] && {
                    echo "错误: -b 参数需要指定构建类型";
                    show_usage;
                    exit 1;
                }
                build_type="$2"
                shift
                ;;
            -p|--platform)
                [[ -z "$2" || "$2" == -* ]] && {
                    echo "错误: -p 参数需要指定平台";
                    show_usage;
                    exit 1;
                }
                platform="$2"
                shift
                ;;
            -c|--clean)
                do_clean=true
                ;;
            -h|--help) 
                show_usage 
                ;;
            *) 
                echo "错误: 未知参数 '$1'"
                show_usage
                exit 1
                ;;
        esac
        shift
    done

    # 处理清理操作
    if [ "${do_clean}" = true ]; then
        clean_build_dir
        exit 0
    fi

    validate_arguments
    check_commands || exit 1
    build_project || exit 1
}

# 执行主函数
main "$@"