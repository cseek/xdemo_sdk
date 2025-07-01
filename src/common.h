/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-20 15:01:24
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-07-01 12:38:02
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <string>

#if defined(XDEMO_SDK_SHARED)
#define XDEMO_SDK_API __attribute__((visibility("default")))
#else
#define XDEMO_SDK_API
#endif

/* 返回码 */
enum class ResCode : uint16_t
{
    SUCCESS = 200,                   // 成功
    ERROR_UNKNOWN = 201,             // 未知错误
    ERROR_CREATE_LOGGER = 202,       // 创建日志器失败
    ERROR_PROCESSING = 203,          // 处理错误
    ERROR_UNSUPPORTED = 204,         // 不支持的操作
    ERROR_SUBSCRIBE = 205,           // 订阅失败
    ERROR_NOT_INITIALIZED = 206,     // 未初始化
    ERROR_INVALID_CONFIG = 207,      // 无效配置
    ERROR_INVALID_CONFIG_PATH = 208, // 无效配置路径
    ERROR_INVALID_LOG_PATH = 209,    // 无效日志路径
    ERROR_INVALID_INPUT = 210,       // 无效输入
};

// GNSS定位数据
struct GnssData
{
    uint64_t frame_id; // 帧ID (递增序号)
    uint64_t sec_week; // GPS周内秒 [s]
    double lat;        // 纬度 [°]
    double lon;        // 经度 [°]
    double alt;        // 海拔高度 [m]
    double lat_std;    // 纬度标准差 [m]
    double lon_std;    // 经度标准差 [m]
    double alt_std;    // 海拔标准差 [m]
};

// IMU惯性测量数据
struct ImuData
{
    uint64_t frame_id; // 帧ID (与GNSS同步)
    uint64_t sec_week; // GPS周内秒 [s]
    double gyro_x;     // X轴角速度 [rad/s]
    double gyro_y;     // Y轴角速度 [rad/s]
    double gyro_z;     // Z轴角速度 [rad/s]
    double acc_x;      // X轴加速度 [m/s²]
    double acc_y;      // Y轴加速度 [m/s²]
    double acc_z;      // Z轴加速度 [m/s²]
};

// 融合后导航数据
struct FusionData
{
    uint64_t frame_id; // 帧ID (融合结果)
    uint64_t sec_week; // GPS周内秒 [s]
    double lat;        // 融合纬度 [°]
    double lon;        // 融合经度 [°]
    double alt;        // 融合海拔 [m]
    double vel_n;      // 北向速度 [m/s]
    double vel_e;      // 东向速度 [m/s]
    double vel_d;      // 地向速度 [m/s]
    double att_r;      // 横滚角 [°]
    double att_p;      // 俯仰角 [°]
    double att_y;      // 偏航角 [°]
};

struct Config
{
    bool enable_console_log; // 打开终端日志
    uint32_t log_level;      // 日志级别 0: TRACE, 1: DEBUG, 2: INFO, 3: WARN, 4: ERROR, 5: CRITICAL, 6: OFF
    uint32_t log_size;       // 日志大小, 单位字节
    uint32_t log_rotation;   // 日志回滚数量
    std::string log_fname;   // 日志文件名，例如: /var/log/test.log
};

#endif // __COMMON_H__