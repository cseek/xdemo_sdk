/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-23 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-08 21:14:39
 * @Description: 这是一个 SDK 的使用示例
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"

int main()
{
    LogConfig logcfg = {
        .path = "./log",                 // 日志存放路径
        .size = 1024 * 1024 * 5,         // 与留给日志的磁盘空间大小 MB
        .level = LogConfig::Level::TRACE // 日志级别
    };
    XDemoSDK sdk;
    sdk.init(DataSource::INTERNAL, logcfg);
    return 0;
}