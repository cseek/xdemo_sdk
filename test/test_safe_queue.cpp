/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-24 12:48:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-24 13:27:51
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "doctest.h"
#include "safe_queue.h"
#include <thread>

TEST_SUITE("test_safe_queue")
{
    TEST_CASE("test_safe_queue::push")
    {
        SafeQueue<int> queue;
        queue.push(1);
        int value;
        CHECK(queue.try_pop(value));
        CHECK(value == 1);
    }
    TEST_CASE("test_safe_queue::pop")
    {
        SafeQueue<int> queue;
        queue.push(2);
        int value;
        CHECK(queue.pop(value));
        CHECK(value == 2);
    }
    TEST_CASE("test_safe_queue::try_pop")
    {
        SafeQueue<int> queue;
        queue.push(3);
        int value;
        CHECK(queue.try_pop(value));
        CHECK(value == 3);
    }
    TEST_CASE("test_safe_queue::size")
    {
        SafeQueue<int> queue;
        queue.push(4);
        CHECK(queue.size() == 1);
        int value;
        queue.pop(value);
        CHECK(queue.size() == 0);
    }
    TEST_CASE("test_safe_queue::clear")
    {
        SafeQueue<int> queue;
        queue.push(5);
        queue.clear();
        CHECK(queue.size() == 0);
        int value;
        CHECK(!queue.try_pop(value));
    }
    TEST_CASE("test_safe_queue::push_full")
    {
        SafeQueue<int> queue(2);
        queue.push(1);
        queue.push(2);
        queue.push(3);
        int value;
        CHECK(queue.try_pop(value));
        CHECK(value == 1);
        CHECK(queue.size() == 1);
    }
    TEST_CASE("test_safe_queue::pop_empty")
    {
        SafeQueue<int> queue;
        int value;
        CHECK(!queue.try_pop(value));
    }
    TEST_CASE("test_safe_queue::push_and_pop_in_threads")
    {
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
        t1.join();
        t2.join();
        CHECK(queue.size() == 0);
    }
}