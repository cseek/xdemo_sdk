/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-01 19:25:51
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-02 00:31:04
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __MACROFUNC_H__
#define __MACROFUNC_H__

#include <iostream>
#include <cstring>

#define Likely(x) __builtin_expect(!!(x), 1)
#define Unlikely(x) __builtin_expect(!!(x), 0)
#define Max(_A, _B) (((_A) > (_B)) ? (_A) : (_B))                              // 取两者的最大值
#define Min(_A, _B) (((_A) < (_B)) ? (_A) : (_B))                              // 取两者的最小值
#define Zero(_name) memset(&_name, 0, sizeof(_name))                           // 清空结构体
#define ArrayNum(x) ((unsigned)(sizeof(x) / sizeof((x)[0])))                   // 计算数组元素个数，传入数组名称
#define ToStr(_text) (#_text)                                                  // 将输入内容字符串化
#define BetweenOf(_val, _min, _max) (((_min) <= (_val)) && ((_val) <= (_max))) // 判断val是否在某个闭区间内

/**
 * @description: 内存安全的字符拷贝
 * @return {*}
 */
#define StrArrncpy(_dest, _src)                  \
    do                                           \
    {                                            \
        strncpy(_dest, _src, sizeof(_dest) - 1); \
        ((char *)_dest)[sizeof(_dest) - 1] = 0;  \
    } while (0)

#endif // __MACROFUNC_H__