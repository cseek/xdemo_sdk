/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2024-06-01 23:21:39
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2024-06-20 00:47:16
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/filesys.h"
#include "doctest.h"
#include <fstream>
#include <string>

TEST_SUITE("test_filesys")
{
    TEST_CASE("test_filesys::delete_file_and_file_exists")
    {
        std::string filename("./mytest.txt");
        std::string content("Hello, World!");
        CHECK(FileSys::file_exists(filename) == false);
        CHECK(FileSys::delete_file(filename) == true);
        CHECK(FileSys::write_file(filename, content) == content.length());
        CHECK(FileSys::delete_file(filename) == true);
        CHECK(FileSys::file_exists(filename) == false);
    }

    TEST_CASE("test_filesys::get_file_size")
    {
        std::string filename("./mytest.txt");
        std::string content("Hello, World!");
        CHECK(FileSys::get_file_size(filename) == 0);
        CHECK(FileSys::write_file(filename, content) == content.length());
        CHECK(FileSys::get_file_size(filename) == content.length());
        CHECK(FileSys::delete_file(filename) == true);
    }

    TEST_CASE("test_filesys::read_file")
    {
        std::string filename("./mytest.txt");
        std::string content("Hello, World!");
        CHECK(FileSys::write_file(filename, content) == content.length());
        CHECK(FileSys::read_file(filename) == content);
        CHECK(FileSys::delete_file(filename) == true);
    }

    TEST_CASE("test_filesys::write_file")
    {
        std::string filename("./mytest.txt");
        std::string content("Hello, World!");
        CHECK(FileSys::write_file(filename, content) == content.length());
        CHECK(FileSys::read_file(filename) == content);
        CHECK(FileSys::delete_file(filename) == true);
    }

    TEST_CASE("test_filesys::directory_exists_and_create_directory_and_delete_directory")
    {
        std::string dirname("./mytestdir");
        CHECK(FileSys::create_directory(dirname) == true);
        CHECK(FileSys::directory_exists(dirname) == true);
        CHECK(FileSys::delete_directory(dirname) == true);
        CHECK(FileSys::directory_exists(dirname) == false);
    }

    TEST_CASE("test_filesys::get_current_directory_and_set_current_directory")
    {
        std::string current_dir = FileSys::get_current_directory();
        CHECK(current_dir.empty() == false);
        CHECK(FileSys::set_current_directory(current_dir) == true);
        CHECK(FileSys::get_current_directory() == current_dir);
    }

    TEST_CASE("test_filesys::calculate_directory_size_recursive")
    {
        std::string dir_top("./mytestdir");
        std::string dir_mid = dir_top + "/subdir";
        std::string dir_bottom = dir_mid + "/subsubdir";
        std::string file_top = dir_top + "/file_top.txt";
        std::string file_mid = dir_mid + "/file_mid.txt";
        std::string file_bottom = dir_bottom + "/file_bottom.txt";
        std::string file_top_content("Hello, Top!");
        std::string file_mid_content("Hello, Mid!");
        std::string file_bottom_content("Hello, Bottom!");
        auto len = file_top_content.length() + file_mid_content.length() + file_bottom_content.length();

        CHECK(FileSys::directory_exists(dir_top) == false);
        CHECK(FileSys::directory_exists(dir_mid) == false);
        CHECK(FileSys::directory_exists(dir_bottom) == false);
        CHECK(FileSys::create_directory(dir_top) == true);
        CHECK(FileSys::create_directory(dir_mid) == true);
        CHECK(FileSys::create_directory(dir_bottom) == true);
        CHECK(FileSys::directory_exists(dir_top) == true);
        CHECK(FileSys::directory_exists(dir_mid) == true);
        CHECK(FileSys::directory_exists(dir_bottom) == true);
        CHECK(FileSys::write_file(file_top, file_top_content) == file_top_content.length());
        CHECK(FileSys::write_file(file_mid, file_mid_content) == file_mid_content.length());
        CHECK(FileSys::write_file(file_bottom, file_bottom_content) == file_bottom_content.length());
        CHECK(FileSys::calculate_directory_size_recursive(dir_top) == len);
        CHECK(FileSys::delete_directory(dir_top) == true);
        CHECK(FileSys::directory_exists(dir_top) == false);
        CHECK(FileSys::directory_exists(dir_mid) == false);
        CHECK(FileSys::directory_exists(dir_bottom) == false);
    }
}