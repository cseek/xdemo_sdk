/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-24 14:06:11
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-24 14:19:53
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "doctest.h"
#include "singleton.h"
#include <thread>
#include <string>

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