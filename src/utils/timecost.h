/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 14:37:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-29 20:09:11
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __TIME_COST_H__
#define __TIME_COST_H__

#include <cstdint>
#include <chrono>
#include <functional>

class TimeCost
{
public:
    using Callback = std::function<void(double cost)>;
    using hrc = std::chrono::high_resolution_clock;
    enum class Unit : uint8_t
    {
        S = 0,  // 秒
        MS = 1, // 毫秒
        US = 2, // 微秒
        NS = 3  // 纳秒
    };

    explicit TimeCost(const Callback &cbk, const Unit &unit = Unit::MS)
        : m_cbk(cbk),
          m_unit(unit),
          m_start_time(hrc::now()) {}

    ~TimeCost()
    {
        auto end_time = hrc::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - m_start_time).count();
        switch (m_unit)
        {
        case Unit::S:
            m_cbk(duration / (1000.0 * 1000.0 * 1000.0));
            break;

        case Unit::MS:
            m_cbk(duration / (1000.0 * 1000.0));
            break;

        case Unit::US:
            m_cbk(duration / 1000.0);
            break;

        case Unit::NS:
            m_cbk(duration);
            break;

        default:
            break;
        }
    }

private:
    Unit m_unit;
    Callback m_cbk;
    std::chrono::time_point<hrc> m_start_time;
};

#endif // __TIME_COST_H__
