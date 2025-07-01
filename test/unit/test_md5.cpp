/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-05-24 15:39:57
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-07-01 12:27:06
 * @Description: 
 * Copyright (c) 2025 by Aurson, All Rights Reserved. 
 */
#include "utils/md5.h"
#include "doctest.h"
#include <string>
#include <cstring>

TEST_SUITE("test_md5")
{
    TEST_CASE("test_md5:md5_file") {
        std::string filename("test_md5.txt");
        std::string content("abcdefghijklmnopqrstuvwxyz");
        FILE *file = fopen(filename.c_str(), "w");
        if (file == nullptr) {
            perror("Failed to create test file");
            return;
        }
        fwrite(content.c_str(), sizeof(char), content.size(), file);
        fclose(file);
        std::string expected_md5 = "c3fcd3d76192e4007dfb496cca67e13b";
        std::string md5_value = md5_file(filename);
        CHECK(md5_value == expected_md5);
        remove(filename.c_str());
    }
    TEST_CASE("test_md5:md5_data") {
        unsigned char data[] = "abcdefghijklmnopqrstuvwxyz";
        std::string expected_md5 = "c3fcd3d76192e4007dfb496cca67e13b";
        std::string md5_value = md5_data(data, sizeof(data) - 1);
        CHECK(md5_value == expected_md5);
    }
}