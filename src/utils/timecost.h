/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 14:37:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-05-24 15:42:32
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __TIME_COST_H__
#define __TIME_COST_H__

#include <chrono>
#include <iostream>
#include <string>

class TimeCost
{
public:
    enum class Unit : uint8_t
    {
        S = 0,  // 秒
        MS = 1, // 毫秒
        US = 2, // 微秒
        NS = 3  // 纳秒
    };
    TimeCost(const std::string &m_tag, const Unit &unit = Unit::MS)
        : m_tag(m_tag), m_unit(unit),
          m_start_time(std::chrono::high_resolution_clock::now()) {}

    ~TimeCost()
    {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - m_start_time).count();
        if (Unit::S == m_unit)
        {
            std::cout << m_tag << " cost: " << duration / (1000.0 * 1000.0 * 1000.0) << " s" << std::endl;
        }
        else if (Unit::MS == m_unit)
        {
            std::cout << m_tag << " cost: " << duration / (1000.0 * 1000.0) << " ms" << std::endl;
        }
        else if (Unit::NS == m_unit)
        {
            std::cout << m_tag << " cost: " << duration / 1000.0 << " us" << std::endl;
        }
        else
        {
            std::cout << m_tag << " cost: " << duration << " ns" << std::endl;
        }
    }

private:
    Unit m_unit;
    std::string m_tag;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
};

#endif // __TIME_COST_H__
