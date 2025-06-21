/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-31 23:50:36
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-01 22:06:03
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __THREAD_H__
#define __THREAD_H__

#include <thread>
#include <string>

namespace Thread
{
    /**
     * @description: 更改线程名
     * @param {string} &name: 线程名
     * @return {bool} true: 成功; false: 失败
     */    
    inline bool set_thread_name(const std::string &name)
    {
        pthread_t thread = pthread_self();
        return pthread_setname_np(thread, name.c_str()) == 0;
    }

    /**
     * @description: 获取当前线程名
     * @return {std::string} 线程名，失败返回空字符串
     */    
    inline std::string get_thread_name()
    {
        char name[50];
        pthread_t thread = pthread_self();
        if (pthread_getname_np(thread, name, sizeof(name)) == 0)
        {
            return std::string(name);
        }
        return {};
    }

    /**
     * @description: 获取当前线程ID
     * @return {std::thread::id} 当前线程ID
     */
    inline auto get_thread_id() -> std::thread::id
    {
        return std::this_thread::get_id();
    }

    /**
     * @description: 绑定当前线程到指定的CPU核心
     * @param {int} core_id: CPU核心ID
     * @return {bool} true: 成功; false: 失败
     */
    inline bool bind_thread_to_core(int core_id)
    {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_id, &cpuset);
        pthread_t thread = pthread_self();
        return pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset) == 0;
    }

    /**
     * @description: 获取当前线程的CPU核心ID
     * @return {int} CPU核心ID，失败返回-1
     */
    inline int get_thread_core_id()
    {
        cpu_set_t cpuset;
        pthread_t thread = pthread_self();
        if (pthread_getaffinity_np(thread, sizeof(cpu_set_t), &cpuset) == 0)
        {
            for (int i = 0; i < CPU_SETSIZE; ++i)
            {
                if (CPU_ISSET(i, &cpuset))
                {
                    // 返回第一个设置的核心ID
                    return i;
                }
            }
        }
        // 如果没有设置，返回-1
        return -1;
    }
}

#endif // __THREAD_H__