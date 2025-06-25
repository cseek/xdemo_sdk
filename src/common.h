/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-20 15:01:24
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-25 22:19:06
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>

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

struct GnssData
{
    uint64_t frame_id;   // 帧ID
    uint64_t timestamp;  // 时间戳（ms）
    double lat;          // 纬度 (度)
    double lon;          // 经度 (度)
    double alt;          // 海拔高度 (m)
    double speed;        // 速度 (m/s)
    double heading;      // 航向角 (度)
    double accuracy;     // 精度
    int satellite_count; // 卫星数量
};

typedef struct
{
    uint64_t frame_id;  // 帧ID
    uint64_t timestamp; // 时间戳（ms）
    double gyro_x;      // X轴角速度 (rad/s)
    double gyro_y;      // Y轴角速度
    double gyro_z;      // Z轴角速度
    double acc_x;       // X轴加速度 (m/s²)
    double acc_y;       // Y轴加速度
    double acc_z;       // Z轴加速度
    double mag_x;       // X轴磁场强度(uT)
    double mag_y;       // Y轴磁场强度
    double mag_z;       // Z轴磁场强度
    double temperature; // 温度 (°C)
} ImuData;

struct WheelData
{
    uint64_t frame_id;          // 帧ID
    uint64_t timestamp;         // 时间戳（ms）
    double front_left;          // 左前轮转速 (rpm)
    double front_right;         // 右前轮转速 (rpm)
    double rear_left;           // 左后轮转速 (rpm)
    double rear_right;          // 右后轮转速 (rpm)
    double steering_angle;      // 转向角度 (度)
    double wheel_circumference; // 车轮周长 (m)
};

struct FusionData
{
    uint64_t frame_id;       // 帧ID
    uint64_t timestamp;      // 时间戳（ms）
    double velocity_e;       // 东向速度 (m/s)
    double velocity_n;       // 北向速度 (m/s)
    double velocity_u;       // 上向速度 (m/s)
    double attitude_r;       // 横滚角 (度)
    double attitude_p;       // 俯仰角 (度)
    double attitude_y;       // 偏航角 (度)
    double position_lat;     // 纬度 (度)
    double position_lon;     // 经度 (度)
    double position_alt;     // 海拔高度 (m)
    double position_lat_std; // 纬度标准差 (度)
    double position_lon_std; // 经度标准差 (度)
    double position_alt_std; // 海拔高度标准差 (m)
};

#endif // __COMMON_H__