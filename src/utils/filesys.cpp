/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-31 23:47:58
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-08 15:49:51
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
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
    bool file_exists(const std::string &filename)
    {
        struct stat st;
        return (stat(filename.c_str(), &st) == 0);
    }

    size_t get_file_size(const std::string &filename)
    {
        struct stat st;
        if (stat(filename.c_str(), &st) != 0)
        {
            return 0;
        }
        return static_cast<size_t>(st.st_size);
    }

    auto read_file(const std::string &filename) -> std::string
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

    ssize_t write_file(const std::string &filename, const std::string &content)
    {
        // 使用更通用的权限设置 (0644)
        int fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC,
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (fd < 0)
        {
            return -1;
        }

        size_t total_written = 0;
        const size_t content_size = content.size();
        while (total_written < content_size)
        {
            ssize_t bytes_written = write(fd, content.data() + total_written, content_size - total_written);
            if (bytes_written < 0)
            {
                if (errno == EINTR)
                { // 处理信号中断
                    continue;
                }
                break; // 其他错误退出循环
            }

            total_written += bytes_written;
        }

        (void)close(fd);
        return static_cast<ssize_t>(total_written);
    }

    bool delete_file(const std::string &filename)
    {
        if (!file_exists(filename))
        {
            return true;
        }
        return (unlink(filename.c_str()) == 0);
    }

    bool directory_exists(const std::string &dirname)
    {
        struct stat st;
        if (stat(dirname.c_str(), &st) != 0)
        {
            return false;
        }
        return S_ISDIR(st.st_mode);
    }

    bool create_directory(const std::string &dirname)
    {
        if (directory_exists(dirname))
        {
            return true;
        }
        return (mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0);
    }

    bool delete_directory(const std::string &path)
    {
        auto callback = [](const char *fpath, const struct stat *, int, struct FTW *)
        {
            return remove(fpath); // 删除文件/目录
        };
        return nftw(path.c_str(), callback, 64, FTW_DEPTH | FTW_PHYS) == 0;
    }

    auto get_current_directory() -> std::string
    {
        char buffer[PATH_MAX];
        if (getcwd(buffer, sizeof(buffer)) != nullptr)
        {
            return std::string(buffer);
        }
        return {};
    }

    bool set_current_directory(const std::string &dirname)
    {
        if (!directory_exists(dirname))
        {
            return false;
        }
        return (chdir(dirname.c_str()) == 0);
    }

    size_t calculate_directory_size_recursive(const std::string &dirname)
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
            {
                continue;
            }
            struct stat st;
            std::string path(dirname);
            path += "/"; // += 优于 append, 优于 +;
            path += entry->d_name;
            if (stat(path.c_str(), &st) == 0)
            {
                if (S_ISREG(st.st_mode))
                {
                    total_size += st.st_size;
                }
                else if (S_ISDIR(st.st_mode))
                {
                    total_size += calculate_directory_size_recursive(path);
                }
            }
        }
        (void)closedir(dir);
        return total_size;
    }
    //////////////////////////////////////////////////
}
