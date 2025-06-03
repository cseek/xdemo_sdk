#!/bin/bash
###
 # @Author: aurson jassimxiong@gmail.com
 # @Date: 2025-05-30 23:50:38
 # @LastEditors: aurson jassimxiong@gmail.com
 # @LastEditTime: 2025-06-03 19:00:27
 # @Description: 
 # Copyright (c) 2025 by Aurson, All Rights Reserved. 
### 
script_path=$(pwd)
report_xslt="$script_path/cppcheck_report.xslt"
report_xml="$script_path/build/cppcheck_report.xml"
report_html="$script_path/build/cppcheck_report.html"
# build
rm "$script_path/build" -r
mkdir -p "$script_path/build"
cd "$script_path/build" || exit
cmake -DCMAKE_BUILD_TYPE=Release \
	-DENABLE_CPPCHECK=ON \
	-DCPPCHECK_REPORT_XML="$report_xml" \
	-DCMAKE_TOOLCHAIN_PATH="${script_path}"/cmake/ToolChainX64.cmake \
	../
make pack || exit
# generate report
xsltproc "$report_xslt" "$report_xml" > "$report_html" || exit
echo "静态分析报告: $report_html"
# open the report in a web browser
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