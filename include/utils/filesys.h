/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-31 23:47:58
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-08 15:47:52
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __FILESYS_H__
#define __FILESYS_H__

#include <string>
#include <cstddef>

namespace FileSys
{
    ///////////// FILE ////////////////////////////////
    /**
     * @description: 检查文件是否存在
     * @param {string} &filename: 文件名
     * @return {bool} true: 存在; false: 不存在
     */
    bool file_exists(const std::string &filename);

    /**
     * @description: 获取文件大小，可以获取 2 GB 以上的文件大小
     * @param {string} &filename: 文件名
     * @return {size_t} 文件大小，失败返回 0
     */
    size_t get_file_size(const std::string &filename);

    /**
     * @description: 读取文件内容（TODO: 分块读取，防止大文件内存爆炸）
     * @param {string} &filename: 文件名
     * @return {string} 文件内容，失败返回空字符串
     */
    auto read_file(const std::string &filename) -> std::string;

    /**
     * @description: 写入文件内容（循环写，防止部分写入）
     * @param {string} &filename: 文件名
     * @param {string} &content: 要写入的内容
     * @return {ssize_t} 0～n: 实际写入字节数; -1: 失败
     */
    ssize_t write_file(const std::string &filename, const std::string &content);

    /**
     * @description: 删除文件
     * @param {string} &filename: 文件名
     * @return {bool} true: 成功或文件不存在; false: 失败
     */
    bool delete_file(const std::string &filename);

    ///////////// DIRECTORY ///////////////////////////
    /**
     * @description: 检查目录是否存在
     * @param {string} &dirname: 目录名
     * @return {bool} true: 存在; false: 不存在
     */
    bool directory_exists(const std::string &dirname);

    /**
     * @description: 创建目录
     * @param {string} &dirname: 目录名
     * @return {bool} true: 成功,或者目录已经存在; false: 失败
     */
    bool create_directory(const std::string &dirname);

    /**
     * @description: 删除目录
     * @param {string} &dirname: 目录名
     * @return {bool} true: 成功或者目录不存在; false: 失败
     */

    bool delete_directory(const std::string &path);

    /**
     * @description: 获取当前工作目录
     * @return {std::string} 当前工作目录，失败返回空字符串
     */
    auto get_current_directory() -> std::string;

    /**
     * @description: 设置当前工作目录
     * @param {string} &dirname: 目录名
     * @return {bool} true: 成功; false: 失败或目录不存在
     */
    bool set_current_directory(const std::string &dirname);

    /**
     * @description: 计算目录大小（递归），兼容 d_type 不可用情况
     * @param {string} &dirname: 目录名
     * @return {size_t} 目录大小，失败返回 0
     */
    size_t calculate_directory_size_recursive(const std::string &dirname);
    //////////////////////////////////////////////////
}
#endif // __FILESYS_H__