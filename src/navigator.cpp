/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-16 23:00:56
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-24 18:33:22
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "navigator.h"
#include "topic.h"
#include "xlogger.h"

bool Navigator::init()
{
    m_subscriber.subscribe<GnssData>(GNSS_TOPIC, [&](const GnssData &gnss_data)
                                     { m_gnss_buffer.push(gnss_data); });
    m_subscriber.subscribe<ImuData>(IMU_TOPIC, [&](const ImuData &imu_data)
                                    { m_imu_buffer.push(imu_data); });
    m_subscriber.subscribe<WheelData>(WHEEL_TOPIC, [&](const WheelData &wheel_data)
                                      { m_wheel_buffer.push(wheel_data); });
    m_thread = std::thread(&Navigator::process, this);
    return true;
}

void Navigator::deinit()
{
    m_runging = false;
    m_subscriber.unsubscribe<GnssData>(GNSS_TOPIC);
    m_subscriber.unsubscribe<ImuData>(IMU_TOPIC);
    m_subscriber.unsubscribe<WheelData>(WHEEL_TOPIC);
    m_gnss_buffer.stop();
    m_imu_buffer.stop();
    m_wheel_buffer.stop();
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void Navigator::process()
{
    m_runging = true;
    while (m_runging)
    {
        GnssData gnss_data;
        ImuData imu_data;
        WheelData wheel_data;

        if (m_gnss_buffer.pop(gnss_data) && m_imu_buffer.pop(imu_data) && m_wheel_buffer.pop(wheel_data))
        {
            XLOGI("gnss_data: frame_id={}, timestamp={}, lat={}, lon={}, alt={}, speed={}, heading={}",
                  gnss_data.frame_id, gnss_data.timestamp, gnss_data.lat, gnss_data.lon, gnss_data.alt, gnss_data.speed, gnss_data.heading);
            XLOGI("imu_data: timestamp={}, acceleration_x={}, acceleration_y={}, acceleration_z={}",
                  imu_data.timestamp, imu_data.acceleration_x, imu_data.acceleration_y, imu_data.acceleration_z);
            XLOGI("wheel_data: frame_id={}, timestamp={}, front_left={}, front_right={}, rear_left={}, rear_right={}",
                  wheel_data.frame_id, wheel_data.timestamp, wheel_data.front_left, wheel_data.front_right,
                  wheel_data.rear_left, wheel_data.rear_right);

            FusionData fusion_data;
            fusion_data.timestamp = gnss_data.timestamp; // 假设时间戳一致
            // 进行数据融合处理
            // ...

            m_publisher.publish<FusionData>(FUSION_TOPIC, fusion_data);
        }
        else
        {
            std::this_thread::yield();
        }
    }
}