#!/bin/bash
###
 # @Author: aurson jassimxiong@gmail.com
 # @Date: 2025-05-30 23:50:38
 # @LastEditors: aurson jassimxiong@gmail.com
 # @LastEditTime: 2025-05-31 21:24:38
 # @Description: 
 # Copyright (c) 2025 by Aurson, All Rights Reserved. 
### 
script_path=$(pwd)
test_exe="$script_path/build/test/test_xdemo_sdk"
report_xslt="$script_path/tamplate.xslt"
report_xml="$script_path/build/report.xml"
report_html="$script_path/build/report.html"
# build test executable
mkdir -p build
cd build && cmake -DCMAKE_BUILD_TYPE=Release ../
make test
# run test and generate report
$test_exe --r=xml --o="$report_xml"
xsltproc "$report_xslt" "$report_xml" > "$report_html" || exit
echo "输出单元测试报告: $report_html"
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