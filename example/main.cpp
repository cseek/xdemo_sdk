/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-23 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 22:26:07
 * @Description: 这是一个 SDK 的使用示例
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"
#include <string>
#include <iostream>

int main()
{
    std::string config_path("./xdemo.conf");
    XDemoSDK sdk;
    ResCode res = sdk.init(DataSource::INTERNAL, config_path);
    if (res != SUCCESS)
    {
        return -1;
    }
    // clang-format off
    sdk.set_output_callback(
        [](const Output &output)
        {
            std::cout << "Output received" << std::endl;
        }
    );
    sdk.deinit();
    // clang-format on

    return 0;
}