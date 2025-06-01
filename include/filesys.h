/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-05-31 23:47:58
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-02 00:03:47
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __FILESYS_H__
#define __FILESYS_H__

#include <string>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ftw.h>

namespace FileSys
{
    ///////////// FILE ////////////////////////////////
    /**
     * @description: 检查文件是否存在
     * @param {string} &filename: 文件名
     * @return {bool} true: 存在; false: 不存在
     */
    inline bool file_exists(const std::string &filename)
    {
        struct stat st;
        return (stat(filename.c_str(), &st) == 0);
    }

    /**
     * @description: 获取文件大小，可以获取 2 GB 以上的文件大小
     * @param {string} &filename: 文件名
     * @return {size_t} 文件大小，失败返回 0
     */
    inline size_t get_file_size(const std::string &filename)
    {
        struct stat st;
        if (stat(filename.c_str(), &st) != 0)
        {
            return 0;
        }
        return static_cast<size_t>(st.st_size);
    }

    /**
     * @description: 读取文件内容（TODO: 分块读取，防止大文件内存爆炸）
     * @param {string} &filename: 文件名
     * @return {string} 文件内容，失败返回空字符串
     */
    inline std::string read_file(const std::string &filename)
    {
        if (!file_exists(filename))
        {
            return {};
        }

        int fd = open(filename.c_str(), O_RDONLY);
        if (fd < 0)
        {
            return {};
        }

        struct stat st;
        (void)fstat(fd, &st);
        size_t size = st.st_size;
        std::string content(size, '\0');
        ssize_t bytes_read = read(fd, &content[0], size);
        (void)close(fd);

        if (bytes_read < 0)
        {
            return {};
        }

        content.resize(bytes_read);
        return content;
    }

    /**
     * @description: 写入文件内容（循环写，防止部分写入）
     * @param {string} &filename: 文件名
     * @param {string} &content: 要写入的内容
     * @return {bool} true: 成功; false: 失败
     */
    inline bool write_file(const std::string &filename, const std::string &content)
    {
        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, S_IRUSR | S_IWUSR);
        if (fd < 0)
        {
            return false;
        }

        size_t total_written = 0;
        while (total_written < content.size())
        {
            ssize_t bytes_written = write(fd, content.data() + total_written, content.size() - total_written);
            if (bytes_written <= 0)
            {
                close(fd);
                return false;
            }
            total_written += bytes_written;
        }
        (void)close(fd);
        return true;
    }

    /**
     * @description: 删除文件
     * @param {string} &filename: 文件名
     * @return {bool} true: 成功或文件不存在; false: 失败
     */
    inline bool delete_file(const std::string &filename)
    {
        if (!file_exists(filename))
        {
            return true;
        }
        return (unlink(filename.c_str()) == 0);
    }

    ///////////// DIRECTORY ///////////////////////////
    /**
     * @description: 检查目录是否存在
     * @param {string} &dirname: 目录名
     * @return {bool} true: 存在; false: 不存在
     */
    inline bool directory_exists(const std::string &dirname)
    {
        struct stat st;
        if (stat(dirname.c_str(), &st) != 0)
        {
            return false;
        }
        return S_ISDIR(st.st_mode);
    }

    /**
     * @description: 创建目录
     * @param {string} &dirname: 目录名
     * @return {bool} true: 成功,或者目录已经存在; false: 失败
     */
    inline bool create_directory(const std::string &dirname)
    {
        if (directory_exists(dirname))
        {
            return true;
        }
        return (mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
    }

    /**
     * @description: 删除目录
     * @param {string} &dirname: 目录名
     * @return {bool} true: 成功或者目录不存在; false: 失败
     */

    inline bool delete_directory(const std::string &path)
    {
        auto callback = [](const char *fpath, const struct stat *, int, struct FTW *)
        {
            return remove(fpath); // 删除文件/目录
        };
        return nftw(path.c_str(), callback, 64, FTW_DEPTH | FTW_PHYS) == 0;
    }

    /**
     * @description: 获取当前工作目录
     * @return {std::string} 当前工作目录，失败返回空字符串
     */
    inline std::string get_current_directory()
    {
        char buffer[PATH_MAX];
        if (getcwd(buffer, sizeof(buffer)) != nullptr)
        {
            return std::string(buffer);
        }
        return {};
    }

    /**
     * @description: 设置当前工作目录
     * @param {string} &dirname: 目录名
     * @return {bool} true: 成功; false: 失败或目录不存在
     */
    inline bool set_current_directory(const std::string &dirname)
    {
        if (!directory_exists(dirname))
        {
            return false;
        }
        return (chdir(dirname.c_str()) == 0);
    }

    /**
     * @description: 计算目录大小（递归），兼容 d_type 不可用情况
     * @param {string} &dirname: 目录名
     * @return {size_t} 目录大小，失败返回 0
     */
    inline size_t calculate_directory_size_recursive(const std::string &dirname)
    {
        size_t total_size = 0;
        DIR *dir = opendir(dirname.c_str());
        if (!dir)
        {
            return 0;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            std::string path = dirname + "/" + entry->d_name;
            struct stat st;
            if (stat(path.c_str(), &st) == 0)
            {
                if (S_ISREG(st.st_mode))
                    total_size += st.st_size;
                else if (S_ISDIR(st.st_mode))
                    total_size += calculate_directory_size_recursive(path);
            }
        }
        (void)closedir(dir);
        return total_size;
    }
    //////////////////////////////////////////////////
}
#endif // __FILESYS_H__