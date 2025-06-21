/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-01 22:30:55
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:43:08
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/cmd.h"
#include "doctest.h"

TEST_SUITE("test_cmd")
{
    TEST_CASE("test_cmd::is_safe")
    {
        CHECK(Cmd::is_safe("ls -l"));
        CHECK_FALSE(Cmd::is_safe("rm -rf /"));
        CHECK_FALSE(Cmd::is_safe("dd if=/dev/zero of=/dev/sda"));
        CHECK_FALSE(Cmd::is_safe("mkfs.ext4 /dev/sda1"));
        CHECK_FALSE(Cmd::is_safe("chmod 777 /"));
        CHECK_FALSE(Cmd::is_safe("chown root:root /"));
    }

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