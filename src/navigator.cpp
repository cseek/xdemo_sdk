/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-16 23:00:56
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-07-01 23:40:49
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "navigator.h"
#include "topic.h"
#include "xlogger.h"

bool Navigator::init()
{
    // clang-format off
    m_subscriber.subscribe<GnssData>(GNSS_TOPIC, [&](const GnssData &gnss_data)
    {
        m_gnss_buffer.push(gnss_data);
    });
    m_subscriber.subscribe<ImuData>(IMU_TOPIC, [&](const ImuData &imu_data)
    {
        m_imu_buffer.push(imu_data);
    });
    m_thread = std::thread(&Navigator::process, this);
    // clang-format on
    return true;
}

void Navigator::deinit()
{
    m_runging = false;
    m_subscriber.unsubscribe<GnssData>(GNSS_TOPIC);
    m_subscriber.unsubscribe<ImuData>(IMU_TOPIC);
    m_gnss_buffer.stop();
    m_imu_buffer.stop();
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

        if (m_imu_buffer.try_pop(imu_data))
        {
            XLOGI("[ imu_data] frame_id: {}", imu_data.frame_id);
            XLOGI("[ imu_data] sec_week: {}", imu_data.sec_week);
            XLOGI("[ imu_data] gyro_x  : {}", imu_data.gyro_x);
            XLOGI("[ imu_data] gyro_y  : {}", imu_data.gyro_y);
            XLOGI("[ imu_data] gyro_z  : {}", imu_data.gyro_z);
            XLOGI("[ imu_data] acc_x   : {}", imu_data.acc_x);
            XLOGI("[ imu_data] acc_y   : {}", imu_data.acc_y);
            XLOGI("[ imu_data] acc_z   : {}", imu_data.acc_z);
            m_fusion.process_imu_data(imu_data);
            if (m_gnss_buffer.try_pop(gnss_data))
            {
                XLOGI("[gnss_data] frame_id: {}", gnss_data.frame_id);
                XLOGI("[gnss_data] sec_week: {}", gnss_data.sec_week);
                XLOGI("[gnss_data] lat     : {}", gnss_data.lat);
                XLOGI("[gnss_data] lon     : {}", gnss_data.lon);
                XLOGI("[gnss_data] alt     : {}", gnss_data.alt);
                XLOGI("[gnss_data] lat_std : {}", gnss_data.lat_std);
                XLOGI("[gnss_data] lon_std : {}", gnss_data.lon_std);
                XLOGI("[gnss_data] alt_std : {}", gnss_data.alt_std);
                m_fusion.process_gnss_data(gnss_data);
            }
            m_publisher.publish<FusionData>(FUSION_TOPIC, m_fusion.get_fusion_data(imu_data.sec_week, imu_data.frame_id));
        }

        std::this_thread::yield();
    }
}