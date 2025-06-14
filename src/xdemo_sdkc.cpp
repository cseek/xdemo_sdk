/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-29 15:37:02
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 22:20:57
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "xdemo_sdkc.h"
#include "xdemo_sdk.h"
#include "utils/singleton.h"

XDEMO_SDK_API ResCode xdemo_sdk_init(DataSource data_source, const char *config_path)
{
    return Singleton<XDemoSDK>::instance().init(data_source, config_path);
}

XDEMO_SDK_API ResCode xdemo_sdk_deinit()
{
    return Singleton<XDemoSDK>::instance().deinit();
}

XDEMO_SDK_API ResCode xdemo_sdk_set_output_callback(const OutputCallback *output_callback)
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

XDEMO_SDK_API ResCode xdemo_sdk_input_data1(const void *input1)
{
    if (nullptr == input1)
    {
        return ERROR_INVALID_INPUT;
    }
    return Singleton<XDemoSDK>::instance().input_data1(input1);
}

XDEMO_SDK_API ResCode xdemo_sdk_input_data2(const void *input2)
{
    if (nullptr == input2)
    {
        return ERROR_INVALID_INPUT;
    }
    return Singleton<XDemoSDK>::instance().input_data2(input2);
}