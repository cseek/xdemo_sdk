/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-19 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-24 16:00:14
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "xdemo_sdk.h"

XDEMO_SDK_API int num_add(int a, int b)
{
    return a + b;
}

XDEMO_SDK_API int num_div(int a, int b)
{
    if (b == 0) {
        return -1;
    }
    return a / b;
}