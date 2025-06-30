/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-05 12:14:52
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:45:47
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/safe_ringbuffer.h"
#include "doctest.h"
#include <thread>

TEST_SUITE("test_safe_ringbuffer")
{
    TEST_CASE("test_safe_ringbuffer::push_and_try_pop")
    {
        int value;
        SafeRingBuffer<int, 2> queue;
        CHECK(queue.push(10));
        CHECK(queue.try_pop(value));
        CHECK(value == 10);
    }
    TEST_CASE("test_safe_ringbuffer::size")
    {
        int value;
        SafeRingBuffer<int, 2> queue;
        CHECK(queue.push(4));
        CHECK(queue.size() == 1);
        CHECK(queue.try_pop(value));
        CHECK(queue.size() == 0);
    }
    TEST_CASE("test_safe_ringbuffer::clear")
    {
        int value;
        SafeRingBuffer<int, 2> queue;
        CHECK(queue.push(5));
        queue.clear();
        CHECK(queue.size() == 0);
        CHECK_FALSE(queue.try_pop(value));
    }
    TEST_CASE("test_safe_ringbuffer::push_full")
    {
        int value;
        SafeRingBuffer<int, 4> queue;
        CHECK(queue.push(1) == true);
        CHECK(queue.push(2) == true);
        CHECK(queue.push(3) == true);
        CHECK(queue.push(4) == true);
        CHECK(queue.push(5) == false); // 队列已满
        CHECK(queue.try_pop(value));
        CHECK(value == 1);
        CHECK(queue.size() == 3);
    }
    TEST_CASE("test_safe_ringbuffer::pop_empty")
    {
        int value;
        SafeRingBuffer<int, 2> queue;
        CHECK_FALSE(queue.try_pop(value));
    }
    TEST_CASE("test_safe_ringbuffer::push_and_pop_in_threads")
    {
        // clang-format off
        SafeRingBuffer<int, 8> queue;
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
