/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-14 13:52:02
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:57:26
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "broker/publisher.h"
#include "broker/subscriber.h"
#include "doctest.h"

TEST_SUITE("test_broker")
{
    TEST_CASE("test_broker::subscribe_and_publish")
    {
        Subscriber subscriber;
        Publisher publisher;

        bool callback_called = false;
        auto callback = [&](const int &data) {
            callback_called = true;
            CHECK(data == 42);
        };

        CHECK(subscriber.subscribe<int>("test_topic", callback));
        publisher.publish<int>("test_topic", 42);
        CHECK(callback_called);
        subscriber.unsubscribe<int>("test_topic");
    }

    TEST_CASE("test_broker::unsubscribe")
    {
        Subscriber subscriber;
        Publisher publisher;

        bool callback_called = false;
        auto callback = [&](const int &data) {
            callback_called = true;
        };

        CHECK(subscriber.subscribe<int>("test_topic", callback));
        subscriber.unsubscribe<int>("test_topic");
        publisher.publish<int>("test_topic", 42);

        CHECK_FALSE(callback_called);
    }
}