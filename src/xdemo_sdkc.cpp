/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-29 15:37:02
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-31 22:29:56
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdkc.h"
#include "xdemo_sdk.h"
#include "singleton.h"

XDEMO_SDK_API ResCode init(DataSource data_source, const LogConfig *log_config)
{
    return Singleton<XDemoSDK>::instance().init(data_source, *log_config);
}

XDEMO_SDK_API ResCode deinit()
{
    return Singleton<XDemoSDK>::instance().deinit();
}

XDEMO_SDK_API ResCode set_output_callback(OutputCallback output_callback)
{
    // clang-format off
    return Singleton<XDemoSDK>::instance()
            .set_output_callback(
                [&](const Output &output)
                {
                    if (output_callback)
                    {
                        output_callback(const_cast<Output *>(&output));
                    }
                }
            );
    // clang-format on
}

XDEMO_SDK_API ResCode input_data(Input *input)
{
    if (nullptr == input)
    {
        return ERROR_INVALID_INPUT;
    }
    return Singleton<XDemoSDK>::instance().input_data(*input);
}