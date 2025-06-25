/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-14 15:00:33
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-20 00:39:46
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "common.h"
#include "nlohmann/json.hpp"
#include <string>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class Config
{
public:
    Config() = default;
    ~Config() = default;

    ResCode init(const std::string &config_path)
    {
        try
        {
            std::ifstream config_file(config_path);
            if (!config_file.is_open())
            {
                return ResCode::ERROR_INVALID_CONFIG_PATH;
            }
            json config_json;
            config_file >> config_json;

            m_enable_bagger = config_json["enable_bagger"];
            m_log_path = config_json["logger"]["path"];
            m_log_size = config_json["logger"]["size"];
            m_log_rotate = config_json["logger"]["rotate"];
            m_log_level = config_json["logger"]["level"];

            return ResCode::SUCCESS;
        }
        catch (const std::exception &e)
        {
            return ResCode::ERROR_INVALID_CONFIG;
        }
        return ResCode::SUCCESS;
    }
    bool enable_bagger() const
    {
        return m_enable_bagger;
    }
    auto log_path() const -> const std::string &
    {
        return m_log_path;
    }
    int log_size() const
    {
        return m_log_size;
    }
    int log_rotate() const
    {
        return m_log_rotate;
    }
    int log_level() const
    {
        return m_log_level;
    }

private:
    bool m_enable_bagger = false;        // 是否启用录包器
    std::string m_log_path = "/var/log"; // 日志文件路径
    int m_log_size = 3;                  // 单个日志文件大小
    int m_log_rotate = 5;                // 总日志文件数量，也是滚动日志的数量
    int m_log_level = 2;                 // 0: TRACE,   最精细的日志，可以用来追溯函数整个运行流程
                                         // 1: DEBUG,   调试信息
                                         // 2: INFO,    一般信息
                                         // 3 WARN,     警告信息
                                         // 4 ERROR,    错误信息
                                         // 5 CRITICAL, 严重错误信息
                                         // 6 OFF       关闭日志
};

#endif // __CONFIG_H__
