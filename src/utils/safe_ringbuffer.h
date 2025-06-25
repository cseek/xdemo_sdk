/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-05 11:16:12
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-11 23:04:23
 * @Description: 环形缓冲区，适用于高性能数据传输场景，支持多线程安全读写操作。但是数据太大的话，
 *               建议使用 safe_queue.h，因为环形缓冲区分配在栈上，栈空间有限，而本工程里的安全
 *               队列是分配在堆上。环形缓冲区设计必须强制容量为2的幂次，这是性能与正确性的关键保障。
 * ==================================================================================
 *             关键点	说明
 *             性能	    位运算比取模快10-30倍
 *             正确性	确保索引正确回绕
 *             硬件友好	充分利用CPU位操作指令
 *             内存优化	自然对齐缓存行
 *             强制约束	需静态断言验证
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __SAFE_RINGBUFFER_H__
#define __SAFE_RINGBUFFER_H__

#include <array>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

template <typename T, size_t CAPACITY>
class SafeRingBuffer
{
    /* 为了高效实现索引回绕操作，通过位运算替代取模运算， 所以缓冲区容量必须大于 0 且是 2 的 n 次方，
    否则会索引计算错误或者缓冲区访问越界*/
    static_assert(CAPACITY > 0 && (CAPACITY & (CAPACITY - 1)) == 0,
                  "Buffer capacity must be a power of two and greater than zero");

public:
    SafeRingBuffer()
        : m_head(0),
          m_tail(0),
          m_count(0)
    {
    }

    ~SafeRingBuffer()
    {
        stop();
    }

    /**
     * @description: 停止所有操作，唤醒等待线程
     * @return {void}
     */
    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_stop)
            {
                return;
            }
            m_stop = true;
        }
        m_cv_not_empty.notify_all();
    }

    /**
     * @description: 非阻塞写入数据（缓冲区满时直接失败）
     * @param {const T&} value 要写入的数据
     * @return {bool} 是否写入成功
     */
    bool push(const T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_count == CAPACITY || m_stop)
        {
            return false;
        }

        m_buffer[m_head] = value;
        m_head = (m_head + 1) & (CAPACITY - 1);
        m_count++;

        m_cv_not_empty.notify_one();
        return true;
    }

    /**
     * @description: 阻塞读取数据
     * @param {T&} value 存储读取的数据
     * @return {bool} 是否读取成功（false表示已停止）
     */
    bool pop(T &value)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv_not_empty.wait(lock, [this]()
                            { return m_count > 0 || m_stop; });

        if (m_stop && m_count == 0)
        {
            return false;
        }

        value = m_buffer[m_tail];
        m_tail = (m_tail + 1) & (CAPACITY - 1);
        m_count--;
        return true;
    }

    /**
     * @description: 非阻塞读取数据
     * @param {T&} value 存储读取的数据
     * @return {bool} 是否读取成功
     */
    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_count == 0)
        {
            return false;
        }

        value = m_buffer[m_tail];
        m_tail = (m_tail + 1) & (CAPACITY - 1);
        m_count--;
        return true;
    }

    /**
     * @description: 获取当前元素数量
     * @return {size_t} 元素数量
     */
    size_t size() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_count;
    }

    /**
     * @description: 清空缓冲区
     * @return {void}
     */
    void clear()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_head = 0;
        m_tail = 0;
        m_count = 0;
    }

    /**
     * @description: 检查缓冲区是否为空
     * @return {bool} 是否为空
     */
    bool empty() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_count == 0;
    }

    /**
     * @description: 检查缓冲区是否已满
     * @return {bool} 是否已满
     */
    bool full() const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_count == CAPACITY;
    }

private:
    std::array<T, CAPACITY> m_buffer;       // 存储数据的环形缓冲区
    size_t m_head = 0;                      // 写入位置
    size_t m_tail = 0;                      // 读取位置
    size_t m_count = 0;                     // 当前元素数量
    bool m_stop = false;                    // 停止标志
    mutable std::mutex m_mutex;             // 互斥锁
    std::condition_variable m_cv_not_empty; // 非空条件变量
};

#endif // __SAFE_RINGBUFFER_H__