/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-10 22:19:03
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-10 22:45:35
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __CPU_USAGE_H__
#define __CPU_USAGE_H__

#include <unistd.h>
#include <sys/times.h>
#include <chrono>

class CpuUsage
{
public:
    CpuUsage()
    {
        m_last_update = std::chrono::steady_clock::now();
        m_prev_proc_time = get_process_time();
    }
    /**
     * @description: 获取CPU使用率
     * @return {double} 返回CPU使用率百分比
     * @note: 该方法返回的CPU使用率是自上次调用以来的平均值
     */
    double get_usage()
    {
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - m_last_update).count();
        m_last_update = now;
        long curr_proc_time = get_process_time();
        long proc_diff = curr_proc_time - m_prev_proc_time;
        m_prev_proc_time = curr_proc_time;
        // 避免除以零
        if (elapsed <= 0 || proc_diff <= 0)
        {
            return 0.0;
        }
        double usage = proc_diff / static_cast<double>(sysconf(_SC_CLK_TCK)) / elapsed;
        return (usage * 100.0);
    }

private:
    /**
     * @description: 获取当前进程的CPU时间
     * @return {long} 返回当前进程的CPU时间（用户时间 + 系统时间）
     */
    long get_process_time()
    {
        struct tms buf;
        times(&buf);
        return buf.tms_utime + buf.tms_stime;
    }

private:
    std::chrono::steady_clock::time_point m_last_update;
    long m_prev_proc_time;
};

#endif // __CPU_USAGE_H__

// 使用示例代码
/*
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <cmath>

void busy_work()
{
    volatile double d = 1.0;
    while (true)
    {
        // 模拟繁忙工作
        for (double j = 0; j < 1000000; ++j)
        {
            d += std::sin(j) * std::cos(j);
        }
        // 为了避免编译器优化掉这个循环
        if (d < 0.1)
        {
            d = 1.0;
        }
    }
}

int main()
{
    CpuUsage cpu_usage;
    // 第一次调用通常返回0
    std::cout << "CPU usage: " << cpu_usage.get_usage() << "%\n";
    // 创建两个繁忙线程
    std::thread(busy_work).detach();
    std::thread(busy_work).detach();
    // 线程结束后
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "CPU usage: " << cpu_usage.get_usage() << "%\n";
    }
    return 0;
}
*/