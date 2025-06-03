/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-20 15:01:24
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-03 08:33:09
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(XDEMO_SDK_SHARED)
#define XDEMO_SDK_API __attribute__((visibility("default")))
#else
#define XDEMO_SDK_API
#endif

// 数据源
enum DataSource
{
    INTERNAL = 0, // 内部数据源
    EXTERNAL = 1  // 外部数据源
};

// 输入的数据
struct Input
{
};

// 输出的数据
struct Output
{
};

// 配置
struct LogConfig
{
    char path[512];   // 日志存放路径
    int size;         // 留给日志文件的空间大小，单位为 MB
    enum Level        // 日志级别
    {                 //
        TRACE = 0,    // 最精细的日志，可以用来追溯函数整个运行流程
        DEBUG = 1,    // 调试信息
        INFO = 2,     // 一般信息
        WARN = 3,     // 警告信息
        ERROR = 4,    // 错误信息
        CRITICAL = 5, // 严重错误信息
        OFF = 6       // 关闭日志
    } level;
};

// 返回码
enum ResCode
{
    SUCCESS = 200,              // 成功
    ERROR_UNKNOWN = 201,        // 未知错误
    ERROR_INVALID_INPUT = 202,  // 无效输入
    ERROR_PROCESSING = 203,     // 处理错误
    ERROR_UNSUPPORTED = 204,    // 不支持的操作
    ERROR_NO_NEED = 205,        // 不需要的操作
    ERROR_NOT_INITIALIZED = 206 // 未初始化
};

#endif // __COMMON_H__