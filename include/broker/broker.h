/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-14 13:22:09
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 13:29:09
 * @Description: 消息代理类，支持主题订阅和发布
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __BROKER_H__
#define __BROKER_H__

#include <functional>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <string>

template <typename DataType>
class Broker
{
public:
    using Topic = std::string;
    using Callback = std::function<void(const DataType &)>;
    /**
     * @description: 订阅函数
     * @param {const Topic &} 要订阅的主题
     * @param {const Callback &} 回调函数
     * @return {bool} true: 订阅成功，false: 主题已存在
     */
    bool subscribe(const Topic &topic, const Callback &callback)
    {
        std::unique_lock<std::shared_mutex> slock(m_mutex);
        auto it = m_callbacks.find(topic);
        if (it == m_callbacks.end())
        {
            m_callbacks[topic] = callback;
            return true;
        }
        return false;
    }

    /**
     * @description: 取消订阅函数
     * @param {const Topic &} 要取消订阅的主题
     */
    void unsubscribe(const Topic &topic)
    {
        std::unique_lock<std::shared_mutex> slock(m_mutex);
        m_callbacks.erase(topic);
    }

    /**
     * @description: 发布函数
     * @param {const Topic &} 要发布的主题
     * @param {const DataType &} 要发布的数据
     */
    void publish(const Topic &topic, const DataType &data)
    {
        std::shared_lock<std::shared_mutex> slock(m_mutex);
        auto it = m_callbacks.find(topic);
        if (it != m_callbacks.end())
        {
            it->second(data);
        }
    }

private:
    std::shared_mutex m_mutex; // 读写锁，允许多个读操作或一个写操作
    std::unordered_map<Topic, Callback> m_callbacks;
};

#endif // __BROKER_H__