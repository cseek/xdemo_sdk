/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 12:17:47
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-14 13:39:52
 * @Description: 线程安全的队列，数据存储在堆上，适合用于大数据量的场景，比如图像领域
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __SAFE_QUEUE_H__
#define __SAFE_QUEUE_H__

#include <cstdint>
#include <queue>
#include <atomic>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue
{
public:
    explicit SafeQueue(uint32_t cap = 200)
        : m_cap(cap),
          m_stop(false)
    {
    }

    ~SafeQueue()
    {
        stop();
    }

    /**
     * @description: 向队列中添加元素
     * @param {T} value: 要添加的元素
     * @return {bool} true: push成功，false: push失败（队列已满）
     */
    bool push(const T &value)
    {
        std::lock_guard<std::mutex> glck(m_mutex);
        if (m_queue.size() >= m_cap)
        {
            // Queue is full;
            return false;
        }

        m_queue.push(value);
        m_cv.notify_one();
        return true;
    }

    /**
     * @description: 尝试从队列中弹出元素
     * @param {T} value: 弹出的元素
     * @return {bool} true: pop成功，false: pop失败
     */
    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> glck(m_mutex);
        if (m_queue.empty())
        {
            return false;
        }
        value = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }

    /**
     * @description: 使用while循环等待队列不为空防止虚假唤醒（spurious wakeup）
     *               当多个线程等待在条件变量上时, 一个线程被唤醒后可能发现队列为空，
     *               所以需要循环检查队列是否为空, 如果队列为空，pop会阻塞当前线程，直到有数据可用或者stop被调用
     * @return {bool} true: pop成功，false: 停止pop
     */
    bool pop(T &value)
    {
        std::unique_lock<std::mutex> ulck(m_mutex);
        while (m_queue.empty() && !m_stop.load())
        {
            m_cv.wait(ulck, [this]()
                      { return !m_queue.empty() || m_stop.load(); }); // 阻塞当前线程，并释放锁
        }
        if (m_stop.load())
        {
            return false;
        }
        value = std::move(m_queue.front());
        m_queue.pop();
        return true;
    }
    /**
     * @description: 停止队列的操作
     * @return {void}
     */
    void stop()
    {
        if (m_stop.load())
        {
            return;
        }
        m_stop.store(true);
        m_cv.notify_all();
        clear();
    }

    /**
     * @description: 清空队列
     * @return {void}
     */
    void clear()
    {
        std::lock_guard<std::mutex> glck(m_mutex);
        while (!m_queue.empty())
        {
            m_queue.pop();
        }
    }

    /**
     * @description: 获取队列的大小
     * @return {size_t} 队列的大小
     */
    size_t size()
    {
        std::lock_guard<std::mutex> glck(m_mutex);
        return m_queue.size();
    }

private:
    uint32_t m_cap;               // 容量
    std::queue<T> m_queue;        // 存储数据的队列
    std::mutex m_mutex;           // 互斥锁，保证对队列的访问是线程安全的
    std::condition_variable m_cv; // 条件变量，用于实现线程间的同步
    std::atomic<bool> m_stop;     // 用于唤醒等待, 退出线程
};

#endif // __SAFE_QUEUE_H__