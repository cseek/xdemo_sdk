/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-13 06:28:41
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-17 20:33:54
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __SUBSCRIBER_H__
#define __SUBSCRIBER_H__

#include "broker/make_id.h"
#include "broker/broker.h"
#include "utils/singleton.h"

class Subscriber
{
public:
    Subscriber()
    {
        m_id = Singleton<MakeId>::instance().request();
    }
    /**
     * @description: 订阅主题
     * @param {const Topic &} topic 要订阅的主题
     * @param {const Callback &} callback 订阅回调函数
     */
    template <typename DataType>
    bool subscribe(const typename Broker<DataType>::Topic &topic,
                   const typename Broker<DataType>::Callback &callback)
    {
        return Singleton<Broker<DataType>>::instance().subscribe(m_id, topic, callback);
    }

    /**
     * @description: 取消订阅的主题
     * @param {const Topic &} topic 要取消订阅的主题
     */
    template <typename DataType>
    void unsubscribe(const typename Broker<DataType>::Topic &topic)
    {
        Singleton<Broker<DataType>>::instance().unsubscribe(m_id, topic);
    }

    uint64_t id() const
    {
        return m_id;
    }

private:
    uint64_t m_id;
};
#endif // __SUBSCRIBER_H__