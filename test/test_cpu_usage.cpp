/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-11 00:10:58
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:43:20
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/cpu_usage.h"
#include "doctest.h"
#include <thread>

TEST_SUITE("test_cpu_usage")
{
    TEST_CASE("test_cpu_usage::get_usage")
    {
        CpuUsage cpu_usage;
        double usage = cpu_usage.get_usage();
        CHECK(usage >= 0.0);
        auto function = []()
        {
            long i = 10000000000;
            while (--i)
            {
                // 模拟CPU负载
                volatile long x = i / 2 * i;
            }
        };
        std::thread(function).detach(); // 启动一个线程模拟CPU负载
        std::thread(function).detach(); // 再启动一个线程模拟CPU负载
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        CHECK(cpu_usage.get_usage() > 180);
        CHECK(cpu_usage.get_usage() < 201);
    }
}