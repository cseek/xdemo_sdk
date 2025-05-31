/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-30 13:22:52
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-31 21:45:15
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "doctest.h"
#include "xlogger.h"
#include <string>
#include <cstring>

TEST_SUITE("test_logger")
{
    TEST_CASE("test_logger::multiple_logger")
    {
        Xlogger logger1, logger2;
        bool init1 = logger1.init("logger1", "/tmp/logger1.log", "[%Y-%m-%d %H:%M:%S.%f] [%^%L%$] %v", 4, 1024 * 1024 * 6, spdlog::level::debug, spdlog::level::warn);
        CHECK(init1 == true);
        bool init2 = logger2.init("logger2", "/tmp/logger2.log", "[%Y-%m-%d %H:%M:%S.%f] [%^%L%$] %v", 4, 1024 * 1024 * 6, spdlog::level::debug, spdlog::level::warn);
        CHECK(init2 == true);
        std::remove("/tmp/logger1.log");
        std::remove("/tmp/logger2.log");
    }
}