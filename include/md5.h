/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 14:54:14
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-05-24 15:21:38
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#ifndef __MD5_H__
#define __MD5_H__

#include <cstdint>
#include <string>

/**
 * @description: 计算文件的MD5值
 * @param {string} &filename
 * @return {*}
 */
auto md5_file(const std::string &filename) -> std::string;

/**
 * @description: 计算数据的MD5值
 * @param {const unsigned char *} data
 * @param {size_t} len
 * @return {*}
 */
auto md5_data(unsigned char *data, size_t len) -> std::string;

#endif // __MD5_H__