/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-23 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-26 01:50:35
 * @Description: 这是一个 SDK 的使用示例
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"
#include "utils/timer.h"
#include "utils/xcsv.h"
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

    GnssData gnss_data;
    ImuData imu_data ;
    WheelData wheel_data;
    CsvRow csv_imu;
    CsvRow csv_gnss;
    CsvReader read_imu("./dataset/input/ADIS16465.csv", ' ');
    CsvReader read_gnss("./dataset/input/GNSS_RTK.csv", ' ');
    read_imu.read_row(csv_imu);
    read_gnss.read_row(csv_gnss);

    Timer timer(5, [&] {
        csv_imu.clear();
        csv_gnss.clear();
        read_imu.read_row(csv_imu);
        read_gnss.read_row(csv_gnss);
        // TODO： string to number
        // 这里面的所有逻辑放到回归测试里去做
        // 这里只作为一个demo而以
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
    read_imu.close();
    read_gnss.close();
    sdk.deinit();

    return 0;
}