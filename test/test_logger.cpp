/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-30 13:22:52
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-20 00:54:07
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "xlogger.h"
#include "doctest.h"
#include <string>
#include <cstring>

TEST_SUITE("test_logger")
{
    TEST_CASE("test_logger::multiple_logger")
    {
        Xlogger logger1, logger2;
        bool init1 = logger1.init("logger1", "./logger1.log", 4, 1024 * 1024 * 6, spdlog::level::debug);
        bool init2 = logger2.init("logger2", "./logger2.log", 4, 1024 * 1024 * 6, spdlog::level::debug);
        CHECK(init1 == true);
        CHECK(init2 == true);
        std::remove("./logger1.log");
        std::remove("./logger2.log");
    }
}