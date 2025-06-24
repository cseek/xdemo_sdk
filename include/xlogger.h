/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 14:37:10
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-20 00:45:17
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __XLOGGER_H__
#define __XLOGGER_H__

#include "spdlog/async.h"
#include "spdlog/fmt/bin_to_hex.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "utils/singleton.h"
#ifdef ANDROID
#include "spdlog/sinks/android_sink.h"
#else
#include "spdlog/sinks/stdout_color_sinks.h"
#endif
#include "spdlog/spdlog.h"
#include <memory>

#define XLOGT(fmt, ...) \
    Singleton<Xlogger>::instance().log_trace(fmt, ##__VA_ARGS__)
#define XLOGD(fmt, ...) \
    Singleton<Xlogger>::instance().log_debug(fmt, ##__VA_ARGS__)
#define XLOGI(fmt, ...) \
    Singleton<Xlogger>::instance().log_info(fmt, ##__VA_ARGS__)
#define XLOGW(fmt, ...) \
    Singleton<Xlogger>::instance().log_warn(fmt, ##__VA_ARGS__)
#define XLOGE(fmt, ...) \
    Singleton<Xlogger>::instance().log_error(fmt, ##__VA_ARGS__)
#define XLOGC(fmt, ...) \
    Singleton<Xlogger>::instance().log_critical(fmt, ##__VA_ARGS__)

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

    /**
     * @description: 初始化
     * @param {tag} 日志名，用来区分不同的日志器，如果有多个日志实例，这就很重要
     * @param {file_name} 日志文件名， 形如："./log/app.log"
     * @param {rotation} 滚动日志数量
     * @param {file_size} 单个日志文件大小，单位字节（Byte）
     * @param {level} 日志级别
     * @return {bool} true: 初始化成功，false: 初始化失败
     */
    bool init(const std::string &tag,
            const std::string &file_name,
            size_t rotation,
            size_t file_size,
            spdlog::level::level_enum level)
    {
        if (m_logger)
        {
            return true;
        }
        auto &&function = [&]()
        {
        // 创建sinks
#ifdef ANDROID
            auto stdout_sink = std::make_shared<spdlog::sinks::android_sink_mt>();
#else
            auto stdout_sink = std::make_shared<stdout_sink_t>();
#endif
            auto rotating_sink = std::make_shared<rotating_sink_t>(file_name, file_size, rotation);
            std::vector<spdlog::sink_ptr> sinks{stdout_sink, rotating_sink};
            // 创建线程池
            m_thread_pool = std::make_shared<spdlog::details::thread_pool>(8192, 1);
            // 创建异步日志
            m_logger = std::make_shared<spdlog::async_logger>(tag, sinks.begin(), sinks.end(), m_thread_pool, spdlog::async_overflow_policy::block);
            // 设置日志样式
            m_logger->set_pattern(spdlog::fmt_lib::format("[%Y-%m-%d %H:%M:%S.%f]({})[%^%L%$] %v", tag));
            // 设置日志级别
            m_logger->set_level(level);
            // 当打印这个级别日志时flush
            m_logger->flush_on(spdlog::level::warn);
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
    inline void log_trace(const char *fmt, Args... args)
    {
        if (m_logger)
        {
            m_logger->trace(fmt, args...);
        }
        else
        {
            SPDLOG_TRACE(fmt, args...);
        }
    }

private:
    std::shared_ptr<spdlog::logger> m_logger;
    std::shared_ptr<spdlog::details::thread_pool> m_thread_pool;
    using stdout_sink_t = spdlog::sinks::stdout_color_sink_mt;
    using rotating_sink_t = spdlog::sinks::rotating_file_sink_mt;
};

#endif // __XLOGGER_H__
