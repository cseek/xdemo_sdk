/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-23 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 22:25:38
 * @Description: 这是一个 SDK 的使用示例
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdkc.h"
#include <stdio.h>

void output_callback(const Output *output)
{
    printf("Output received\n");
}

int main()
{
    const char *config_path = "./xdemo.conf";
    ResCode resc = xdemo_sdk_init(INTERNAL, config_path);
    if (resc != SUCCESS)
    {
        return -1;
    }
    xdemo_sdk_set_output_callback(output_callback);
    xdemo_sdk_deinit();
    return 0;
}