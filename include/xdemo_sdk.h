/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-19 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 14:00:33
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
    using OutputCallback = std::function<void(const Output &output)>;

public:
    XDemoSDK(const XDemoSDK &) = delete;
    XDemoSDK &operator=(const XDemoSDK &) = delete;
    XDemoSDK();
    ~XDemoSDK();

public:
    ResCode init(DataSource data_source, const LogConfig &log_config);
    ResCode deinit();
    ResCode set_output_callback(const OutputCallback &output_callback);
    ResCode input_data(const void *input);

private:
    class XDemoSDKImpl;
    std::unique_ptr<XDemoSDKImpl> m_impl;
};

#endif // __XDEMO_SDK_H__