/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 14:37:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-05-24 15:42:37
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include <sys/time.h> // 常用于高精度计时
#include <time.h>     // 常用于格式化时间
#include <cstdint>
#include <string>

/**
 * @description: 获取当前秒级时间戳
 * @return {uint64_t} 当前秒级时间戳
 */
inline auto timestamp_sec() -> uint64_t
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec;
}

/**
 * @description: 获取当前毫秒级时间戳
 * @return {uint64_t} 当前毫秒级时间戳
 */
inline auto timestamp_misec() -> uint64_t
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000LL + tv.tv_usec / 1000;
}

/**
 * @description: 获取当前微秒级时间戳
 * @return {uint64_t} 当前微秒级时间戳
 */
inline auto timestamp_usec() -> uint64_t
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

/**
 * @description: 获取当前纳秒级时间戳
 * @return {uint64_t} 当前纳秒级时间戳
 */
inline auto timestamp_nsec() -> uint64_t
{
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000000000LL + tv.tv_usec * 1000;
}

/**
 * @description: 获取当前时间戳字符串，格式为YYYYMMDDHHMMSS
 * @return {std::string} 当前时间戳字符串
 */
inline auto get_time_str() -> std::string
{
    struct tm *time_info;
    char timestamp[15] = {0};
    time_t current_time;
    // 获取当前时间
    time(&current_time);
    // 转换为本地时间结构
    time_info = localtime(&current_time);
    // 格式化时间戳为指定字符串
    strftime(timestamp, sizeof(timestamp), "%Y%m%d%H%M%S", time_info);
    return std::string(timestamp);
}

#endif // __TIMESTAMP_H__
