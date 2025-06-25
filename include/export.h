/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-25 12:32:19
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-25 12:36:07
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */

#ifndef __EXPORT_H__
#define __EXPORT_H__

#if defined(XDEMO_SDK_SHARED)
#define XDEMO_SDK_API __attribute__((visibility("default")))
#else
#define XDEMO_SDK_API
#endif

#endif // __EXPORT_H__