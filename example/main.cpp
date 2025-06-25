/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-23 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-25 12:50:27
 * @Description: 这是一个 SDK 的使用示例
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"
#include "utils/timer.h"
#include <signal.h>
#include <string>
#include <iostream>
#include <thread>

bool is_running = true;

void signal_handler(int signum)
{
    is_running = false;
}

int main()
{
    signal(SIGINT, signal_handler);
    XDemoSDK sdk;
    ResCode res = sdk.init("./config/xdemo.conf");
    if (res != ResCode::SUCCESS)
    {
        return -1;
    }
    sdk.set_fusion_data_callback(
        [](const FusionData &fusion_data)
        {
            // std::cout << "Output received" << std::endl;
        }
    );
    GnssData gnss_data = {1, 0, 39.9042, 116.4074, 10.0, 5.0, 90.0, 1.0, 8};
    ImuData imu_data = {1, 0, 0.1, 0.2, 9.81, 0.01, 0.02, 0.03, 30.0};
    WheelData wheel_data = {1, 0, 1.0};

    Timer timer(5, [&] {
        sdk.input_gnss_data(gnss_data);
        sdk.input_imu_data(imu_data);
        sdk.input_wheel_data(wheel_data);
    }, "read_handler");
    timer.start();

    while (is_running)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    timer.stop();
    sdk.deinit();

    return 0;
}