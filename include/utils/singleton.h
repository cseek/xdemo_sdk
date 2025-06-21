/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 12:14:41
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-05-30 13:08:40
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <utility>

template <class T>
class Singleton
{
public:
    template <typename... Args>
    static T &instance(Args &&...args)
    {
        static T obj(std::forward<Args>(args)...);
        return obj;
    }

protected:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton() = default;
    ~Singleton() = default;
};

#endif // __SINGLETON_H__