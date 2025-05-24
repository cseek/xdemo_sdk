/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-24 14:37:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-24 15:42:04
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __DEFINE_H__
#define __DEFINE_H__

#define LOG_TAG "sdk"                                    // 日志tag
#define LOG_FILE_SIZE 1024 * 1024 * 6                    // 单个日志文件大小为6MB
#define LOG_ROTATION 4                                   // 日志文件满4个时开始滚动日志
#define LOG_FLUSH_ON spdlog::level::info                 // 当打印这个级别日志时flush
#define LOG_LEVEL spdlog::level::debug                   // 日志级别
#define LOG_PATTERN "[%Y-%m-%d %H:%M:%S.%f] [%^%L%$] %v" // 日志样式

#endif // __DEFINE_H__
