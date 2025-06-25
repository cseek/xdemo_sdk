/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-10 14:23:39
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:48:39
 * @Description: 定时器类，基于 timerfd 实现，约 0.1 毫秒精度，start 后会马上先触发一次。
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <cstdint>
#include <cstring>
#include <chrono>
#include <cerrno>
#include <thread>
#include <string>
#include <functional>

class Timer
{
public:
    using TaskCallback = std::function<void(void)>;
    /**
     * @description: 定时器构造函数
     * @param ms: 定时器间隔时间，单位毫秒
     * @param callback: 定时器触发时调用的回调函数
     * @param timer_name: 定时器名称，用于调试和日志记录
     */
    Timer(uint64_t ms, const TaskCallback &callback, const std::string &timer_name = "")
        : m_is_running(false),
          m_timer_fd(-1),
          m_interval_ms(ms),
          m_callback(callback),
          m_timer_name(timer_name)
    {
    }
    Timer(const Timer &) = delete;
    Timer &operator=(const Timer &) = delete;

    ~Timer()
    {
        stop();
    }
    /**
     * @description: 启动定时器
     */
    void start()
    {
        if (m_is_running)
        {
            return;
        }

        m_timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
        if (m_timer_fd == -1)
        {
            perror("timerfd_create failed");
            return;
        }

        struct itimerspec its;
        memset(&its, 0, sizeof(its));
        uint64_t ms = m_interval_ms;
        its.it_interval.tv_sec = ms / 1000;
        its.it_interval.tv_nsec = (ms % 1000) * 1000000;
        its.it_value = its.it_interval;

        if (timerfd_settime(m_timer_fd, 0, &its, nullptr) == -1)
        {
            perror("timerfd_settime failed");
            close(m_timer_fd);
            m_timer_fd = -1;
            return;
        }
        m_is_running = true;
        // clang-format off
        m_thread = std::thread([&]()
        {
            if (!m_timer_name.empty())
            {
                (void)pthread_setname_np(pthread_self(), m_timer_name.c_str());
            }

            while (m_is_running)
            {
                uint64_t expirations;
                ssize_t bytes = read(m_timer_fd, &expirations, sizeof(expirations));
                if (bytes == sizeof(expirations))
                {
                    if (expirations > 0)
                    {
                        for (uint64_t i = 0; i < expirations; ++i)
                        {
                            m_callback();
                        }
                    }
                }
                else
                {
                    if (errno == EBADF || !m_is_running)
                    {
                        break;
                    }
                    perror("read timerfd failed");
                }
            }
        });
        // clang-format on
    }
    /**
     * @description: 停止定时器
     */
    void stop()
    {
        if (!m_is_running)
        {
            return;
        }

        m_is_running = false;

        if (m_timer_fd != -1)
        {
            close(m_timer_fd);
            m_timer_fd = -1;
        }

        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }
    /**
     * @description: 获取定时器名
     * @return {std::string &} 返回定时器名称的引用
     */
    auto get_timer_name() const -> const std::string &
    {
        return m_timer_name;
    }

private:
    int m_timer_fd;
    bool m_is_running;
    uint64_t m_interval_ms;
    std::string m_timer_name;
    std::thread m_thread;
    TaskCallback m_callback;
};

#endif // __TIMER_H__
