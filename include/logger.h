/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-24 14:37:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-05-24 15:42:11
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __LOGGER_H__
#define __LOGGER_H__

#include "define.h"
#include "singleton.h"
#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <memory>
#include <iostream>

// 将数组转成 16 进制，然后再打印输出，
// 例如: LOGI("{:X}", TO_HEX(data, len));
#define TO_HEX(data, len) spdlog::to_hex(data, data + len)
#define LOGE(...) Singleton<Logger>::instance().log_error(__VA_ARGS__)
#define LOGW(...) Singleton<Logger>::instance().log_warn(__VA_ARGS__)
#define LOGI(...) Singleton<Logger>::instance().log_info(__VA_ARGS__)
#define LOGD(...) Singleton<Logger>::instance().log_debug(__VA_ARGS__)
#define LOGC(...) Singleton<Logger>::instance().log_critical(__VA_ARGS__)
#define LOG_FLUSH(...) Singleton<Logger>::instance().flush()

class Logger
{
private:
    std::unique_ptr<spdlog::logger> m_logger; // 日至器

public:
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    Logger()
    {
        auto &&function = [&]()
        {
            auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(LOG_TAG, LOG_FILE_SIZE, LOG_ROTATION);
            std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
            m_logger = std::make_unique<spdlog::logger>(LOG_TAG, sinks.begin(), sinks.end());
            m_logger->set_pattern(LOG_PATTERN);
            m_logger->set_level(LOG_LEVEL);
            m_logger->flush_on(LOG_FLUSH_ON);
        };
        try
        {
            function();
        }
        catch (const std::exception &e)
        {
            SPDLOG_ERROR("Construct logger error: {}", e.what());
        }
    }

    void flush()
    {
        m_logger->flush();
    }

    template <typename... Args>
    inline void log_error(const char *fmt, Args... args)
    {
        m_logger->error(fmt, args...);
    }

    template <typename... Args>
    inline void log_warn(const char *fmt, Args... args)
    {
        m_logger->warn(fmt, args...);
    }

    template <typename... Args>
    inline void log_info(const char *fmt, Args... args)
    {
        m_logger->info(fmt, args...);
    }

    template <typename... Args>
    inline void log_debug(const char *fmt, Args... args)
    {
        m_logger->debug(fmt, args...);
    }

    template <typename... Args>
    inline void log_critical(const char *fmt, Args... args)
    {
        m_logger->critical(fmt, args...);
    }
};

#endif // __LOGGER_H__
