/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-19 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-25 22:18:36
 * @Description: SDK 的 C++ 语言接口头文件
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __XDEMO_SDK_H__
#define __XDEMO_SDK_H__

#include "common.h"
#include <memory>
#include <string>
#include <functional>

class XDEMO_SDK_API XDemoSDK
{
public:
    using FusionDataCallback = std::function<void(const FusionData &output)>;

public:
    XDemoSDK(const XDemoSDK &) = delete;
    XDemoSDK &operator=(const XDemoSDK &) = delete;
    XDemoSDK();
    ~XDemoSDK();

public:
    ResCode init(const std::string &config_path);
    ResCode deinit();
    ResCode set_fusion_data_callback(const FusionDataCallback &output_callback);
    ResCode input_gnss_data(const GnssData &gnss_data);
    ResCode input_imu_data(const ImuData &imu_data);
    ResCode input_wheel_data(const WheelData &wheel_data);

private:
    class XDemoSDKImpl;
    std::unique_ptr<XDemoSDKImpl> m_impl;
};

#endif // __XDEMO_SDK_H__