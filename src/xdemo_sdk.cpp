/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-19 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-07-01 01:11:10
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"
#include "monitor.h"
#include "navigator.h"
#include "topic.h"
#include "xlogger.h"
#include "broker/publisher.h"
#include "broker/subscriber.h"
#include "utils/singleton.h"
#include "utils/timestamp.h"
#include <string>

class XDemoSDK::XDemoSDKImpl
{
public:
    XDemoSDKImpl() = default;
    ~XDemoSDKImpl() = default;

    ResCode init(const Config &config)
    {
        using namespace spdlog::level;
        std::string tag("xdemo-sdk");
        int rotation = config.log_rotation;
        int file_size = config.log_size;
        auto level = static_cast<level_enum>(config.log_level);
        auto &file_name = config.log_fname;
        auto &logger_instance = Singleton<Xlogger>::instance();
        if (!logger_instance.init(tag, file_name, rotation, file_size, level))
        {
            XLOGC("Failed to create logger");
            return ResCode::ERROR_CREATE_LOGGER;
        }
        if (!m_subscriber.subscribe<FusionData>(FUSION_TOPIC, std::bind(&XDemoSDKImpl::OutputFusionData, this, std::placeholders::_1)))
        {
            XLOGE("Failed to subscribe to topic: {}", FUSION_TOPIC);
            return ResCode::ERROR_SUBSCRIBE;
        }
        m_navigator.init();
        m_monitor.init();

        return ResCode::SUCCESS;
    }

    ResCode deinit()
    {
        m_monitor.deinit();
        m_navigator.deinit();
        return ResCode::SUCCESS;
    }

    ResCode set_fusion_data_callback(const FusionDataCallback &fusion_data_callback)
    {
        m_fusion_data_callback = fusion_data_callback;
        return ResCode::SUCCESS;
    }

    ResCode input_gnss_data(const GnssData &gnss_data)
    {
        m_publisher.publish<GnssData>(GNSS_TOPIC, gnss_data);
        return ResCode::SUCCESS;
    }

    ResCode input_imu_data(const ImuData &imu_data)
    {
        m_publisher.publish<ImuData>(IMU_TOPIC, imu_data);
        return ResCode::SUCCESS;
    }

    void OutputFusionData(const FusionData &fusion_data)
    {
        if (m_fusion_data_callback)
        {
            m_fusion_data_callback(fusion_data);
        }
    }

private:
    Publisher m_publisher;   // 发布者
    Subscriber m_subscriber; // 订阅者
    Navigator m_navigator;   // 导航器
    Monitor m_monitor;       // 监控器
    XDemoSDK::FusionDataCallback m_fusion_data_callback;
};

XDemoSDK::XDemoSDK()
    : m_impl(new XDemoSDKImpl())
{
}

XDemoSDK::~XDemoSDK()
{
}

ResCode XDemoSDK::init(const Config &config)
{
    if (m_impl)
    {
        return m_impl->init(config);
    }
    return ResCode::ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::deinit()
{
    if (m_impl)
    {
        return m_impl->deinit();
    }
    return ResCode::ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::set_fusion_data_callback(const FusionDataCallback &fusion_data_callback)
{
    if (m_impl)
    {
        return m_impl->set_fusion_data_callback(fusion_data_callback);
    }
    return ResCode::ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::input_gnss_data(const GnssData &gnss_data)
{
    if (m_impl)
    {
        return m_impl->input_gnss_data(gnss_data);
    }
    return ResCode::ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::input_imu_data(const ImuData &imu_data)
{
    if (m_impl)
    {
        return m_impl->input_imu_data(imu_data);
    }
    return ResCode::ERROR_NOT_INITIALIZED;
}