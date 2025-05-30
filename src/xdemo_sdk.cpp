/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-19 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-30 13:29:32
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"
#include "singleton.h"
#include "xlogger.h"

class XDemoSDK::XDemoSDKImpl
{
public:
    XDemoSDKImpl() = default;
    ~XDemoSDKImpl() = default;

    ResCode init(DataSource data_source)
    {
        return SUCCESS;
    }

    ResCode deinit()
    {
        // 反初始化逻辑
        return SUCCESS;
    }

    ResCode set_output_callback(const XDemoSDK::OutputCallback &output_callback)
    {
        m_output_callback = output_callback;
        return SUCCESS;
    }

    ResCode input_data(const Input &input)
    {
        // 输入数据处理逻辑
        return SUCCESS;
    }

    void output(const Output &output)
    {
        if (m_output_callback)
        {
            m_output_callback(output);
        }
    }

private:
    XDemoSDK::OutputCallback m_output_callback;
};

XDemoSDK::XDemoSDK()
    : m_impl(new XDemoSDKImpl())
{
}

XDemoSDK::~XDemoSDK()
{
}

ResCode XDemoSDK::init(DataSource data_source)
{
    if (m_impl)
    {
        return m_impl->init(data_source);
    }
    return ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::deinit()
{
    if (m_impl)
    {
        return m_impl->deinit();
    }
    return ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::set_output_callback(const XDemoSDK::OutputCallback &output_callback)
{
    if (m_impl)
    {
        return m_impl->set_output_callback(output_callback);
    }
    return ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::input_data(const Input &input)
{
    if (m_impl)
    {
        return m_impl->input_data(input);
    }
    return ERROR_NOT_INITIALIZED;
}