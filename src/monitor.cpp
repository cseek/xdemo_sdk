/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-17 00:11:08
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-30 22:02:27
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "monitor.h"
#include "topic.h"
#include "common.h"
#include "xlogger.h"

void Monitor::init()
{
    m_timer = std::make_unique<Timer>(1000, [&]()
    {
        XLOGI("Cpu Usage: {:.2f}%", m_cpu_usage.get_usage());
        XLOGI("gnss_fps: {}", m_gnss_fps);
        XLOGI("imu_fps: {}", m_imu_fps);
        m_gnss_fps = 0;
        m_imu_fps = 0;
     }, "monitor");

    m_subscriber.subscribe<GnssData>(GNSS_TOPIC, [&](const GnssData &gnss_data) { ++m_gnss_fps; });
    m_subscriber.subscribe<ImuData>(IMU_TOPIC, [&](const ImuData &imu_data) { ++m_imu_fps; });
    m_timer->start();
}

void Monitor::deinit()
{
    m_timer->stop();
}