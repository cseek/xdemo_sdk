/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 14:06:11
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:46:04
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "utils/singleton.h"
#include "doctest.h"
#include <thread>

TEST_SUITE("test_singleton")
{
    TEST_CASE("test_singleton:instance")
    {
        auto &instance1 = Singleton<int>::instance();
        instance1 = 42;
        auto &instance2 = Singleton<int>::instance();
        CHECK(&instance1 == &instance2);
        CHECK(instance2 == 42);
    }
}