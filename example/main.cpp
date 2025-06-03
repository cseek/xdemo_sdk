/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-23 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-03 08:33:28
 * @Description: 这是一个 SDK 的使用示例
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "xdemo_sdk.h"
#include <iostream>

int main() {
    XDemoSDK sdk;
    sdk.init(DataSource::INTERNAL, {});
    return 0;
}