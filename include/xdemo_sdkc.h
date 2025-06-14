/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-29 13:30:15
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-14 16:31:41
 * @Description: sdk 的 C 语言接口头文件
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __XDEMO_SDKC_H__
#define __XDEMO_SDKC_H__

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif
    typedef void(OutputCallback)(const Output *output);

    /**
     * @description: 初始化 SDK
     * @param {DataSource} data_source，INTERNAL: 内部数据源，EXTERNAL: 外部数据源
     * @return {ResCode} 返回码
     */
    XDEMO_SDK_API ResCode xdemo_sdk_init(DataSource data_source, const char *config_path);

    /**
     * @description: 反初始化 SDK
     * @return {ResCode} 返回码
     */
    XDEMO_SDK_API ResCode xdemo_sdk_deinit();

    /**
     * @description: 设置输出回调函数
     * @param {OutputCallback*} output_callback
     * @return {ResCode} 返回码
     */
    XDEMO_SDK_API ResCode xdemo_sdk_set_output_callback(OutputCallback *output_callback);

    /**
     * @description: 输入数据1
     * @param {void *} input1
     * @return {ResCode} 返回码
     */
    XDEMO_SDK_API ResCode xdemo_sdk_input_data1(const void *input1);

    /**
     * @description: 输入数据2
     * @param {void *} input2
     * @return {ResCode} 返回码
     */
    XDEMO_SDK_API ResCode xdemo_sdk_input_data2(const void *input2);

#ifdef __cplusplus
}
#endif

#endif // __XDEMO_SDKC_H__