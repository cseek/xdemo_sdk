/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-24 12:14:41
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-24 15:42:28
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <utility>

template <class T>
class Singleton
{
protected:
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;
    Singleton() = default;
    ~Singleton() = default;

public:
    template <typename... Args>
    static T &instance(Args &&...args)
    {
        static T obj(std::forward<Args>(args)...);
        return obj;
    }
};

#endif // __SINGLETON_H__