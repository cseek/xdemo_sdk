/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-02 00:08:18
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:44:31
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/macro_func.h"
#include "doctest.h"
#include <iostream>

static void count()
{
    static int cnt = 0;
    cnt++;
    std::cout << "cnnt called, count: " << cnt << std::endl;
}

TEST_SUITE("test_macro_func")
{
    TEST_CASE("test_macro_func::Likely")
    {
        int x = 1;
        CHECK(Likely(x == 1) == true);
        CHECK(Unlikely(x != 1) == false);
    }

    TEST_CASE("test_macro_func::Max")
    {
        CHECK(Max(3, 5) == 5);
    }

    TEST_CASE("test_macro_func::Min")
    {
        CHECK(Min(3, 5) == 3);
    }

    TEST_CASE("test_macro_func::Zero")
    {
        struct TestStruct
        {
            int a;
            double b;
        };
        TestStruct ts;
        ts.a = 5;
        ts.b = 3.14;
        Zero(ts);
        CHECK(ts.a == 0);
        CHECK(ts.b == 0.0);
    }

    TEST_CASE("test_macro_func::ArrayNum")
    {
        int arr[] = {1, 2, 3, 4, 5};
        CHECK(ArrayNum(arr) == 5);
    }

    TEST_CASE("test_macro_func::ToStr")
    {
        CHECK(std::string(ToStr(Hello)) == "Hello");
    }

    TEST_CASE("test_macro_func::BetweenOf")
    {
        CHECK(BetweenOf(5, 1, 10) == true);
        CHECK(BetweenOf(0, 1, 10) == false);
        CHECK(BetweenOf(11, 1, 10) == false);
    }

    TEST_CASE("test_macro_func::StrArrncpy")
    {
        char dest[10];
        StrArrncpy(dest, "Hello");
        CHECK(std::string(dest) == "Hello");
        StrArrncpy(dest, "HelloWorld");
        CHECK(std::string(dest) == "HelloWorl"); // 超出长度被截断
    }
}