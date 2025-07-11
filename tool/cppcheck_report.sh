#!/bin/bash
###
 # @Author: aurson jassimxiong@gmail.com
 # @Date: 2024-05-30 23:50:38
 # @LastEditors: aurson jassimxiong@gmail.com
 # @LastEditTime: 2024-06-04 23:13:28
 # @Description: 
 # Copyright (c) 2025 by Aurson, All Rights Reserved. 
### 
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
project_dir=${script_dir}/..
build_dir=${project_dir}/build
report_xslt="$project_dir/xslt/cppcheck_report.xslt"
report_xml="$build_dir/cppcheck_report.xml"
report_html="$build_dir/cppcheck_report.html"
build_type="Release"
platform="X64"
echo "正在构建..."
echo "├─ 构建类型: ${build_type}"
echo "└─ 目标平台: ${platform}"
# 编译
rm "$build_dir" -r
cmake -B ${build_dir} \
	  -DENABLE_CPPCHECK=ON \
	  -DCPPCHECK_REPORT_XML="$report_xml" \
      -DCMAKE_TOOLCHAIN_FILE=${project_dir}/cmake/Toolchain${platform}.cmake \
      -DCMAKE_BUILD_TYPE=${build_type} \
      ${project_dir} || exit

cmake --build ${build_dir} --target pack --parallel
# 生成报告
xsltproc "$report_xslt" "$report_xml" > "$report_html" || exit
echo "静态分析报告: $report_html"
# 浏览报告
/bin/python3 << !EOF!
import os, webbrowser, sys
try:
	from urllib import pathname2url
except:
	from urllib.request import pathname2url
def main():
    webbrowser.open("file://" + pathname2url(os.path.abspath("$report_html")))
main()
!EOF!