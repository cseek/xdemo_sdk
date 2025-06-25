/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-31 23:48:55
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-26 00:35:08
 * @Description: 优先使用别的方式，非必要不调用系统命令
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __CMD_H__
#define __CMD_H__

#include <string>

namespace Cmd
{
    /**
     * @description: 执行系统命令不获取输出
     * @param {string} &command: 要执行的命令
     * @return {int} 大于0: 执行成功，-1: 打开管道失败, -2: 关闭管道失败, -3: 执行命令异常终止
     * @note: 命令运行结束的状态值 0~255,大部分执行成功会返回0,但是有的
     *        命令执行成功也会返回非0值,如grep命令没有匹配到内容会返回1
     */
    int execute(const std::string &command);

    /**
     * @description: 执行系统命令并获取输出
     * @param {string} &command: 要执行的命令
     * @param {string} &output: 执行命令的输出结果
     * @return {int} 大于0: 执行成功，-1: 打开管道失败, -2: 关闭管道失败, -3: 执行命令异常终止
     * @note: 命令运行结束的状态值 0~255,大部分执行成功会返回0,但是有的
     *        命令执行成功也会返回非0值,如grep命令没有匹配到内容会返回1
     */
    int execute(const std::string &command, std::string &output);
}

#endif // __CMD_H__