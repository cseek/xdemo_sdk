/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-20 15:01:24
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-16 23:07:13
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

#include <stdint.h>

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
    double latitude;     // 纬度 (度)
    double longitude;    // 经度 (度)
    double altitude;     // 海拔高度 (m)
    double speed;        // 速度 (m/s)
    double heading;      // 航向角 (度)
    double accuracy;     // 精度
    int satellite_count; // 卫星数量
};

typedef struct
{
    uint64_t frame_id;     // 帧ID
    uint64_t timestamp;    // 时间戳（ms）
    double acceleration_x; // X轴加速度 (m/s²)
    double acceleration_y; // Y轴加速度
    double acceleration_z; // Z轴加速度
    double gyro_x;         // X轴角速度 (rad/s)
    double gyro_y;         // Y轴角速度
    double gyro_z;         // Z轴角速度
    double magnetometer_x; // X轴磁场强度(uT)
    double magnetometer_y; // Y轴磁场强度
    double magnetometer_z; // Z轴磁场强度
    double temperature;    // 温度 (°C)
} ImuData;

struct WheelData
{
    uint64_t frame_id;          // 帧ID
    uint64_t timestamp;         // 时间戳（ms）
    double front_left_rpm;      // 左前轮转速 (rpm)
    double front_right_rpm;     // 右前轮转速 (rpm)
    double rear_left_rpm;       // 左后轮转速 (rpm)
    double rear_right_rpm;      // 右后轮转速 (rpm)
    double steering_angle;      // 转向角度 (度)
    double wheel_circumference; // 车轮周长 (m)
};

struct FusionData
{
    uint64_t frame_id;             // 帧ID
    uint64_t timestamp;            // 时间戳（ms）
    double position_latitude;      // 纬度 (度)
    double position_longitude;     // 经度 (度)
    double position_altitude;      // 海拔高度 (m)
    double velocity_east;          // 东向速度 (m/s)
    double velocity_north;         // 北向速度 (m/s)
    double velocity_up;            // 上向速度 (m/s)
    double attitude_roll;          // 横滚角 (度)
    double attitude_pitch;         // 俯仰角 (度)
    double attitude_yaw;           // 偏航角 (度)
    double position_std_latitude;  // 纬度标准差 (度)
    double position_std_longitude; // 经度标准差 (度)
    double position_std_altitude;  // 海拔高度标准差 (m)
};

#endif // __COMMON_H__