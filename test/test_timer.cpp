/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-11 00:28:15
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:47:07
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/timer.h"
#include "doctest.h"
#include <string>

TEST_SUITE("test_timer")
{
    TEST_CASE("test_timer::start_stop")
    {
        int counter = 0;
        std::string timer_name = "test_timer";
        Timer timer(200, [&]() { counter++; }, timer_name);
        timer.start();
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
        timer.stop();
        // 计时器每200毫秒触发一次，600毫秒大约会触发3次, 但是start时会触发一次，所以总共应该是4次
        CHECK(counter == 4);
        CHECK(timer.get_timer_name() == timer_name);
    }
}