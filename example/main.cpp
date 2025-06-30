/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-23 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-07-01 01:06:40
 * @Description: 这是一个 SDK 的使用示例
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"
#include <signal.h>
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
    Config config;

    config.enable_console_log = true;
    config.log_level = 2;
    config.log_size = 20 * 1024 * 1024;
    config.log_rotation = 5;
    config.log_fname = "./build/log/xdemo_sdk.log";

    ResCode res = sdk.init(config);
    if (res != ResCode::SUCCESS)
    {
        return -1;
    }
    sdk.set_fusion_data_callback(
        [](const FusionData &fusion_data)
        {
            std::cout << "Output received" << std::endl;
        }
    );

    int filter = 0;
    GnssData gnss_data;
    ImuData imu_data;

    while (is_running)
    {
        ++filter;
        sdk.input_imu_data(imu_data);
        if (filter >= 200)
        {
            filter = 0;
            sdk.input_gnss_data(gnss_data);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    sdk.deinit();

    return 0;
}