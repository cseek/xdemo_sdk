/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-14 13:24:02
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:24:46
 * @Description: 发布器
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#ifndef __PUBLISHER_H__
#define __PUBLISHER_H__

#include "broker/broker.h"
#include "utils/singleton.h"

class Publisher
{
public:
    /**
     * @description: 发布数据到数据总线
     * @param {DataType} data 要发布的数据
     */
    template <typename DataType>
    void publish(const typename Broker<DataType>::Topic &topic, const DataType &data)
    {
        Singleton<Broker<DataType>>::instance().publish(topic, data);
    }
};

#endif // __PUBLISHER_H__