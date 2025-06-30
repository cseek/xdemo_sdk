/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 12:48:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:45:35
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/safe_queue.h"
#include "doctest.h"
#include <thread>

TEST_SUITE("test_safe_queue")
{
    TEST_CASE("test_safe_queue::push")
    {
        int value;
        SafeQueue<int> queue;
        queue.push(1);
        CHECK(queue.try_pop(value));
        CHECK(value == 1);
    }
    TEST_CASE("test_safe_queue::pop")
    {
        int value;
        SafeQueue<int> queue;
        queue.push(2);
        CHECK(queue.pop(value));
        CHECK(value == 2);
    }
    TEST_CASE("test_safe_queue::try_pop")
    {
        int value;
        SafeQueue<int> queue;
        queue.push(3);
        CHECK(queue.try_pop(value));
        CHECK(value == 3);
    }
    TEST_CASE("test_safe_queue::size")
    {
        int value;
        SafeQueue<int> queue;
        queue.push(4);
        CHECK(queue.size() == 1);
        queue.pop(value);
        CHECK(queue.size() == 0);
    }
    TEST_CASE("test_safe_queue::clear")
    {
        int value;
        SafeQueue<int> queue;
        queue.push(5);
        queue.clear();
        CHECK(queue.size() == 0);
        CHECK(!queue.try_pop(value));
    }
    TEST_CASE("test_safe_queue::push_full")
    {
        int value;
        SafeQueue<int> queue(2);
        CHECK(queue.push(1) == true);
        CHECK(queue.push(2) == true);
        CHECK(queue.push(3) == false); // 队列已满
        CHECK(queue.try_pop(value));
        CHECK(value == 1);
        CHECK(queue.size() == 1);
    }
    TEST_CASE("test_safe_queue::pop_empty")
    {
        int value;
        SafeQueue<int> queue;
        CHECK(!queue.try_pop(value));
    }
    TEST_CASE("test_safe_queue::push_and_pop_in_threads")
    {
        // clang-format off
        SafeQueue<int> queue;
        std::thread t1([&]()
        {
            for (int i = 0; i < 5; ++i)
            {
                queue.push(i);
            }
        });
        std::thread t2([&]()
        {
            for (int i = 0; i < 5; ++i)
            {
                int value;
                if (queue.pop(value))
                {
                    CHECK(value >= 0);
                    CHECK(value <= 5);
                }
            }
        });
        // clang-format on
        t1.join();
        t2.join();
        CHECK(queue.size() == 0);
    }
}