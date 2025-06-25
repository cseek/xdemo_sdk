/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-25 23:34:26
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-26 00:26:29
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/xcsv.h"
#include "utils/filesys.h"
#include "doctest.h"
#include <string>
#include <algorithm>

TEST_SUITE("test_xcsv")
{
    TEST_CASE("test_xcsv::write_and_read-1")
    {
        std::string fname("output.csv");
        // 默认以 ',' 分割的
        CSVWriter writer(fname);
        CsvData wdata = {
            {"Name", "Age", "City"},
            {"Alice", "25", "NewYork"},
            {"Bob", "30", "London"}
        };
        CHECK(writer.write(wdata));
        CSVReader reader(fname);
        CsvData rdata;
        CHECK(reader.read(rdata));
        CHECK(wdata==rdata);
        CHECK(FileSys::delete_file(fname));
    }
    
    TEST_CASE("test_xcsv::write_and_read-2")
    {
        std::string fname("output.csv");
        // 以空格分割的
        CSVWriter writer(fname, ' ');
        CsvData wdata = {
            {"Name", "Age", "City"},
            {"Alice", "25", "NewYork"},
            {"Bob", "30", "London"}
        };
        CHECK(writer.write(wdata));
        CSVReader reader(fname, ' ');
        CsvData rdata;
        CHECK(reader.read(rdata));
        CHECK(wdata==rdata);
        CHECK(FileSys::delete_file(fname));
    }
}