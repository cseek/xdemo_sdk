/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-01 22:08:19
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:46:56
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/thread.h"
#include "doctest.h"
#include <iostream> // for std::thread::id comparison

TEST_SUITE("test_thread")
{
    TEST_CASE("test_thread::set_thread_name_and_get_thread_name")
    {
        Thread::set_thread_name("TestThread");
        CHECK(Thread::get_thread_name() == "TestThread");
    }

    TEST_CASE("test_thread::get_thread_id")
    {
        auto id = Thread::get_thread_id();
        CHECK(id == std::this_thread::get_id());
    }

    TEST_CASE("test_thread::bind_thread_to_core_and_get_thread_core_id")
    {
        int core_id = 1;
        bool result = Thread::bind_thread_to_core(core_id);
        CHECK(result == Thread::get_thread_core_id());
    }
}