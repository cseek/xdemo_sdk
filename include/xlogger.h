/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-24 14:37:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-02 10:55:45
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __XLOGGER_H__
#define __XLOGGER_H__

#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <memory>

class Xlogger
{
public:
    Xlogger(const Xlogger &) = delete;
    Xlogger &operator=(const Xlogger &) = delete;
    Xlogger() = default;
    ~Xlogger()
    {
        if (m_logger)
        {
            m_logger->flush();
        }
    }

    bool init(const std::string &name,                                           // 日志器名称
              const std::string &file = "./log/app.log",                         // 日志文件名
              const std::string &pattern = "[%Y-%m-%d %H:%M:%S.%f] [%^%L%$] %v", // 日志样式
              size_t rotation = 4,                                               // 日志文件满4个时开始滚动日志
              size_t file_size = 1024 * 1024 * 6,                                // 单个日志文件大小为6MB
              spdlog::level::level_enum level = spdlog::level::debug,            // 日志级别
              spdlog::level::level_enum flush_on = spdlog::level::warn)          // 当打印这个级别日志时flush
    {
        if (m_logger)
        {
            return true;
        }
        auto &&function = [&]()
        {
            auto stdout_sink = std::make_shared<stdout_sink_t>();
            auto rotating_sink = std::make_shared<rotating_sink_t>(file, file_size, rotation);
            std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
            m_logger = std::make_unique<spdlog::logger>(name, sinks.begin(), sinks.end());
            m_logger->set_pattern(pattern);
            m_logger->set_level(level);
            m_logger->flush_on(flush_on);
            spdlog::flush_every(std::chrono::seconds(1));
        };
        try
        {
            function();
            return true;
        }
        catch (const std::exception &e)
        {
            SPDLOG_ERROR("Construct logger error: {}", e.what());
            return false;
        }
    }

    inline void flush()
    {
        if (m_logger)
        {
            m_logger->flush();
        }
    }

    template <typename... Args>
    inline void log_error(const char *fmt, Args... args)
    {
        if (m_logger)
        {
            m_logger->error(fmt, args...);
        }
        else
        {
            SPDLOG_ERROR(fmt, args...);
        }
    }

    template <typename... Args>
    inline void log_warn(const char *fmt, Args... args)
    {
        if (m_logger)
        {
            m_logger->warn(fmt, args...);
        }
        else
        {
            SPDLOG_WARN(fmt, args...);
        }
    }

    template <typename... Args>
    inline void log_info(const char *fmt, Args... args)
    {
        if (m_logger)
        {
            m_logger->info(fmt, args...);
        }
        else
        {
            SPDLOG_INFO(fmt, args...);
        }
    }

    template <typename... Args>
    inline void log_debug(const char *fmt, Args... args)
    {
        if (m_logger)
        {
            m_logger->debug(fmt, args...);
        }
        else
        {
            SPDLOG_DEBUG(fmt, args...);
        }
    }

    template <typename... Args>
    inline void log_critical(const char *fmt, Args... args)
    {
        if (m_logger)
        {
            m_logger->critical(fmt, args...);
        }
        else
        {
            SPDLOG_CRITICAL(fmt, args...);
        }
    }

private:
    using stdout_sink_t = spdlog::sinks::stdout_color_sink_mt;
    using rotating_sink_t = spdlog::sinks::rotating_file_sink_mt;
    std::unique_ptr<spdlog::logger> m_logger; // 日志器
};

#endif // __XLOGGER_H__
