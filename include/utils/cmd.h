/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-31 23:48:55
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-01 23:12:37
 * @Description: 优先使用别的方式，非必要不调用系统命令
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __CMD_H__
#define __CMD_H__

#include <string>
#include <stdio.h>
#include <sys/wait.h>

namespace Cmd
{
    /**
     * @description: 检查命令是否安全
     * @param {string} &command: 要执行的命令
     * @return {bool} true: 安全, false: 不安全
     */
    inline bool is_safe(const std::string &command)
    {
        return command.find("rm") == std::string::npos &&
               command.find("dd") == std::string::npos &&
               command.find("mkfs") == std::string::npos &&
               command.find("chmod") == std::string::npos &&
               command.find("chown") == std::string::npos;
    }

    /**
     * @description: 执行系统命令不获取输出
     * @param {string} &command: 要执行的命令
     * @return {int} 大于0: 执行成功，-1: 命令不安全, -2: 打开管道失败, -3: 关闭管道失败, -4: 执行命令异常终止
     * @note: 命令运行结束的状态值 0~255,大部分执行成功会返回0,但是有的
     *        命令执行成功也会返回非0值,如grep命令没有匹配到内容会返回1
     */
    inline int execute(const std::string &command)
    {
        if (!is_safe(command))
        {
            return -1;
        }
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe)
        {
            return -2;
        }
        // 读取并丢弃所有输出，防止缓冲区溢出导致 SIGPIPE
        char buffer[512];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr);
        auto status = pclose(pipe);
        if (-1 == status)
        {
            return -3;
        }
        if (!WIFEXITED(status))
        {
            return -4;
        }
        return WEXITSTATUS(status); // 命令运行结束的状态值 0~255
    }

    /**
     * @description: 执行系统命令并获取输出
     * @param {string} &command: 要执行的命令
     * @param {string} &output: 执行命令的输出结果
     * @return {int} 大于0: 执行成功，-1: 命令不安全, -2: 打开管道失败, -3: 关闭管道失败, -4: 执行命令异常终止
     * @note: 命令运行结束的状态值 0~255,大部分执行成功会返回0,但是有的
     *        命令执行成功也会返回非0值,如grep命令没有匹配到内容会返回1
     */
    inline int execute(const std::string &command, std::string &output)
    {
        if (!is_safe(command))
        {
            return -1;
        }
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe)
        {
            return -2;
        }

        char buffer[512];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        {
            output.append(buffer);
        }

        auto status = pclose(pipe);
        if (-1 == status)
        {
            return -3;
        }
        if (!WIFEXITED(status))
        {
            return -4;
        }
        return WEXITSTATUS(status); // 命令运行结束的状态值 0~255
    }
}

#endif // __CMD_H__