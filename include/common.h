/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-20 15:01:24
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-24 15:59:20
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(XDEMO_SDK_SHARED)
#define XDEMO_SDK_API __attribute__((visibility("default")))
#else
#define XDEMO_SDK_API
#endif

#endif // __COMMON_H__