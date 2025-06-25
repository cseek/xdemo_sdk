/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-01 22:30:55
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-26 01:03:09
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/cmd.h"
#include "doctest.h"

TEST_SUITE("test_cmd")
{
    TEST_CASE("test_cmd::execute")
    {
        std::string output;
        CHECK(Cmd::execute("ls -l", output) >= 0);              // 执行成功
        CHECK(output.empty() == false);                         // 输出不为空
        output.clear();                                         // 清空输出
        CHECK(Cmd::execute("ls -l | grep total", output) >= 0); // 执行成功
        CHECK(output.compare("total"));                         // 输出不为空
        CHECK(Cmd::execute("ls -l") >= 0);                      // 执行成功
        CHECK(Cmd::execute("ls -l | grep total") >= 0);         // 执行成功
    }
}