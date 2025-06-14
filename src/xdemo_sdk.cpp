/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-19 23:23:35
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 22:20:20
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdk.h"
#include "config.h"
#include "xlogger.h"
#include "utils/singleton.h"
#include <string>

class XDemoSDK::XDemoSDKImpl
{
public:
    XDemoSDKImpl() = default;
    ~XDemoSDKImpl() = default;

    ResCode init(DataSource data_source, const std::string &config_path)
    {
        // 加载配置
        auto &config_instance = Singleton<Config>::instance();
        ResCode res = config_instance.init(config_path);
        if (ResCode::SUCCESS != res)
        {
            XLOGC("Failed to load config from path: {}", config_path);
            return res;
        }
        // 创建日志器
        std::string logger_name("xdemo_sdk"); // 用来区分不同日志器
        std::string file(config_instance.log_path() + logger_name);
        std::string pattern(spdlog::fmt_lib::format("[{}] [%Y-%m-%d %H:%M:%S.%f] [%^%L%$] %v", logger_name));
        int rotation = config_instance.log_rotate();
        int file_size = config_instance.log_size() * 1024 * 1024; // 转换为字节
        auto level = static_cast<spdlog::level::level_enum>(config_instance.log_level());
        auto &logger_instance = Singleton<Xlogger>::instance();
        if (!logger_instance.init(logger_name, file, pattern, rotation, file_size, level))
        {
            return ResCode::ERROR_CREATE_LOGGER;
        }

        return ResCode::SUCCESS;
    }

    ResCode deinit()
    {
        // 反初始化逻辑
        return SUCCESS;
    }

    ResCode set_output_callback(const OutputCallback &output_callback)
    {
        m_output_callback = output_callback;
        return SUCCESS;
    }

    ResCode input_data1(const void *input1)
    {
        if (INTERNAL == m_data_source)
        {
            return ERROR_NO_NEED;
        }
        return SUCCESS;
    }

    ResCode input_data2(const void *input2)
    {
        if (INTERNAL == m_data_source)
        {
            return ERROR_NO_NEED;
        }
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
    DataSource m_data_source;
    XDemoSDK::OutputCallback m_output_callback;
};

XDemoSDK::XDemoSDK()
    : m_impl(new XDemoSDKImpl())
{
}

XDemoSDK::~XDemoSDK()
{
}

ResCode XDemoSDK::init(DataSource data_source, const std::string &config_path)
{
    if (m_impl)
    {
        return m_impl->init(data_source, config_path);
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

ResCode XDemoSDK::set_output_callback(const OutputCallback &output_callback)
{
    if (m_impl)
    {
        return m_impl->set_output_callback(output_callback);
    }
    return ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::input_data1(const void *input1)
{
    if (m_impl)
    {
        return m_impl->input_data1(input1);
    }
    return ERROR_NOT_INITIALIZED;
}

ResCode XDemoSDK::input_data2(const void *input2)
{
    if (m_impl)
    {
        return m_impl->input_data2(input2);
    }
    return ERROR_NOT_INITIALIZED;
}