/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-29 13:30:15
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-04 18:09:52
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
    typedef void (OutputCallback)(const Output *output);

    /**
     * @description: 初始化 SDK
     * @param {DataSource} data_source，INTERNAL: 内部数据源，EXTERNAL: 外部数据源
     * @return {ResCode} 返回码
     */    
    XDEMO_SDK_API ResCode init(DataSource data_source, const LogConfig *log_config);

    /**
     * @description: 反初始化 SDK
     * @return {ResCode} 返回码
     */
    XDEMO_SDK_API ResCode deinit();
    
    /**
     * @description: 设置输出回调函数
     * @param {OutputCallback} output_callback
     * @return {ResCode} 返回码
     */    
    XDEMO_SDK_API ResCode set_output_callback(const OutputCallback *output_callback);

    /**
     * @description: 输入数据
     * @param {Input} *input
     * @return {ResCode} 返回码
     */    
    XDEMO_SDK_API ResCode input_data(const Input *input);

#ifdef __cplusplus
}
#endif

#endif // __XDEMO_SDKC_H__