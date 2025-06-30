/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-16 22:59:03
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-30 21:59:10
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __NAVIGATOR_H__
#define __NAVIGATOR_H__

#include "common.h"
#include "broker/publisher.h"
#include "broker/subscriber.h"
#include "utils/safe_ringbuffer.h"
#include <thread>

class Navigator
{
public:
    bool init();
    void deinit();

private:
    void process();

private:
    bool m_runging = true;                        // 运行标志
    std::thread m_thread;                         // 处理线程
    Publisher m_publisher;                        // 发布者
    Subscriber m_subscriber;                      // 订阅者
    SafeRingBuffer<GnssData, 32> m_gnss_buffer;   // GNSS 数据缓冲区
    SafeRingBuffer<ImuData, 32> m_imu_buffer;     // IMU 数据缓冲区
};

#endif // __NAVIGATOR_H__