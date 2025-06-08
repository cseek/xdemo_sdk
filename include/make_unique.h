/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-08 17:45:08
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-08 17:45:34
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#ifndef __MAKE_UNIQUE_H__
#define __MAKE_UNIQUE_H__

#if __cplusplus >= 201402L  // C++14 or later

#include <memory>
using std::make_unique;

#else  // C++11

#include <memory>
#include <type_traits>

namespace detail {

// 基础版本 (非数组类型)
template <typename T, typename... Args>
typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique_helper(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 数组版本 (T[])
template <typename T>
typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0, 
                        std::unique_ptr<T>>::type
make_unique_helper(std::size_t size) {
    using element_type = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new element_type[size]());
}

// 禁用定长数组 (T[N])
template <typename T, typename... Args>
typename std::enable_if<std::extent<T>::value != 0, void>::type
make_unique_helper(Args&&...) = delete;

}  // namespace detail

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return detail::make_unique_helper<T>(std::forward<Args>(args)...);
}

#endif  // __cplusplus < 201402L
#endif // __MAKE_UNIQUE_H__