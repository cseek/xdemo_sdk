/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-17 00:06:16
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-22 16:51:59
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __MONITOR_H__
#define __MONITOR_H__

#include "utils/timer.h"
#include "utils/cpu_usage.h"
#include "broker/subscriber.h"
#include <memory>

class Monitor
{
public:
    Monitor(const Monitor &) = delete;
    Monitor &operator=(const Monitor &) = delete;
    Monitor() = default;
    ~Monitor() = default;

public:
    void init();
    void deinit();

private:
    int m_gnss_fps = 0;
    int m_imu_fps = 0;
    int m_wheel_fps = 0;
    CpuUsage m_cpu_usage;
    Subscriber m_subscriber;
    std::unique_ptr<Timer> m_timer;
};
#endif // __MONITOR_H__