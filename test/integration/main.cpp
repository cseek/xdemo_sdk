/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-19 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-07-01 23:50:37
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */

#include "xdemo_sdk.h"
#include "utils/timer.h"
#include "utils/xcsv.h"
#include <signal.h>
#include <string>

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

    sdk.init(config);
    CsvWriter write_fusion("./dataset/csv/fusion.csv", ' ');
    CsvRow row = {"frame_id", "sec_week", "lat(deg)", "lon(deg)", "alt", "vel_n", "vel_e", "vel_d", "att_r", "att_p", "att_y"};

    write_fusion.write_row(row);
    // clang-format off
    sdk.set_fusion_data_callback(
    [&](const FusionData &fusion_data)
    {
        row.clear();
        row =
        {
            std::to_string(fusion_data.frame_id),
            std::to_string(fusion_data.sec_week),
            std::to_string(fusion_data.lat),
            std::to_string(fusion_data.lon),
            std::to_string(fusion_data.alt),
            std::to_string(fusion_data.vel_n),
            std::to_string(fusion_data.vel_e),
            std::to_string(fusion_data.vel_d),
            std::to_string(fusion_data.att_r),
            std::to_string(fusion_data.att_p),
            std::to_string(fusion_data.att_y)
        };
        write_fusion.write_row(row);
    });
    // clang-format on
    GnssData gnss_data;
    ImuData imu_data;
    CsvRow csv_imu;
    CsvRow csv_gnss;
    CsvReader read_imu("./dataset/csv/ADIS16465.csv", ' ');
    CsvReader read_gnss("./dataset/csv/GNSS_RTK.csv", ' ');
    uint64_t gnss_frame_id = 0;
    uint64_t imu_frame_id = 0;
    uint32_t filter = 0;

    read_imu.read_row(csv_imu); // 读出首行
    read_gnss.read_row(csv_gnss);

    Timer timer(5, [&]
                {
        csv_imu.clear();
        csv_gnss.clear();
        if (read_imu.read_row(csv_imu)) // 200 HZ
        {
            imu_data.frame_id = imu_frame_id++;
            imu_data.sec_week = std::stod(csv_imu[0]);
            imu_data.gyro_x = std::stod(csv_imu[1]);
            imu_data.gyro_y = std::stod(csv_imu[2]);
            imu_data.gyro_z = std::stod(csv_imu[3]);
            imu_data.acc_x = std::stod(csv_imu[4]);
            imu_data.acc_y = std::stod(csv_imu[5]);
            imu_data.acc_z = std::stod(csv_imu[6]);
            sdk.input_imu_data(imu_data);
        }
        ++filter;
        if (200 == filter) // 1HZ
        {
            filter = 0;
            if (read_gnss.read_row(csv_gnss))
            {
                gnss_data.frame_id = gnss_frame_id++;
                gnss_data.sec_week = std::stod(csv_gnss[0]);
                gnss_data.lat = std::stod(csv_gnss[1]);
                gnss_data.lon = std::stod(csv_gnss[2]);
                gnss_data.alt = std::stod(csv_gnss[3]);
                gnss_data.lat_std = std::stod(csv_gnss[4]);
                gnss_data.lon_std = std::stod(csv_gnss[5]);
                gnss_data.alt_std = std::stod(csv_gnss[6]);
                sdk.input_gnss_data(gnss_data);
            }
        } }, "read_handler");
    timer.start();

    while (is_running)
    {
#if 0
        csv_imu.clear();
        csv_gnss.clear();
        if (read_imu.read_row(csv_imu)) // 200 HZ
        {
            imu_data.frame_id = imu_frame_id++;
            imu_data.sec_week = std::stod(csv_imu[0]);
            imu_data.gyro_x = std::stod(csv_imu[1]);
            imu_data.gyro_y = std::stod(csv_imu[2]);
            imu_data.gyro_z = std::stod(csv_imu[3]);
            imu_data.acc_x = std::stod(csv_imu[4]);
            imu_data.acc_y = std::stod(csv_imu[5]);
            imu_data.acc_z = std::stod(csv_imu[6]);
            sdk.input_imu_data(imu_data);
        }
        ++filter;
        if (200 == filter) // 1HZ
        {
            filter = 0;
            if (read_gnss.read_row(csv_gnss))
            {
                gnss_data.frame_id = gnss_frame_id++;
                gnss_data.sec_week = std::stod(csv_gnss[0]);
                gnss_data.lat = std::stod(csv_gnss[1]);
                gnss_data.lon = std::stod(csv_gnss[2]);
                gnss_data.alt = std::stod(csv_gnss[3]);
                gnss_data.lat_std = std::stod(csv_gnss[4]);
                gnss_data.lon_std = std::stod(csv_gnss[5]);
                gnss_data.alt_std = std::stod(csv_gnss[6]);
                sdk.input_gnss_data(gnss_data);
            }
        }
#endif
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    timer.stop();
    read_imu.close();
    read_gnss.close();
    sdk.deinit();
}
