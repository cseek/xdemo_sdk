/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-13 06:28:41
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 13:39:31
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __SUBSCRIBER_H__
#define __SUBSCRIBER_H__

#include "broker/broker.h"
#include "utils/singleton.h"

class Subscriber
{
public:
    /**
     * @description: 订阅主题
     * @param {const Topic &} topic 要订阅的主题
     * @param {const Callback &} callback 订阅回调函数
     */
    template <typename DataType>
    bool subscribe(const typename Broker<DataType>::Topic &topic,
                   const typename Broker<DataType>::Callback &callback)
    {
        return Singleton<Broker<DataType>>::instance().subscribe(topic, callback);
    }

    /**
     * @description: 取消订阅的主题
     * @param {const Topic &} topic 要取消订阅的主题
     */
    template <typename DataType>
    void unsubscribe(const typename Broker<DataType>::Topic &topic)
    {
        Singleton<Broker<DataType>>::instance().unsubscribe(topic);
    }
};

#endif // __SUBSCRIBER_H__