/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-20 15:01:24
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 16:59:33
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

/* 数据源 */
typedef enum
{
    INTERNAL = 0, // 内部数据源
    EXTERNAL = 1  // 外部数据源
} DataSource;

/* 输出的数据 */
typedef struct
{
} Output;

/* 返回码 */
typedef enum
{
    SUCCESS = 200,                   // 成功
    ERROR_UNKNOWN = 201,             // 未知错误
    ERROR_CREATE_LOGGER = 202,       // 创建日志器失败
    ERROR_PROCESSING = 203,          // 处理错误
    ERROR_UNSUPPORTED = 204,         // 不支持的操作
    ERROR_NO_NEED = 205,             // 不需要的操作
    ERROR_NOT_INITIALIZED = 206,     // 未初始化
    ERROR_INVALID_CONFIG = 207,      // 无效配置
    ERROR_INVALID_CONFIG_PATH = 208, // 无效配置路径
    ERROR_INVALID_LOG_PATH = 209,    // 无效日志路径
    ERROR_INVALID_INPUT = 210,       // 无效输入
} ResCode;

#endif // __COMMON_H__