/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-25 23:34:26
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-26 01:40:32
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#include "utils/xcsv.h"
#include "utils/filesys.h"
#include "doctest.h"
#include <string>
#include <algorithm>
#include <iostream>

TEST_SUITE("test_xcsv")
{
    TEST_CASE("test_xcsv::write_all_and_read_all-1")
    {
        std::string fname("output.csv");
        // 默认以 ',' 分割的
        CsvWriter writer(fname);
        CsvData wdata = {
            {"Name", "Age", "City"},
            {"Alice", "25", "NewYork"},
            {"Bob", "30", "London"}
        };
        // 写完要关闭文件，否则其他地方无法读
        CHECK(writer.write_all(wdata));
        writer.close();

        CsvReader reader(fname);
        CsvData rdata;
        CHECK(reader.read_all(rdata));
        CHECK_EQ(wdata, rdata);
        reader.close();
        CHECK(FileSys::delete_file(fname));
    }

    TEST_CASE("test_xcsv::write_all_and_read_all-2")
    {
        std::string fname("output.csv");
        // 以空格分割的
        CsvWriter writer(fname, ' ');
        CsvData wdata = {
            {"Name", "Age", "City"},
            {"Alice", "25", "NewYork"},
            {"Bob", "30", "London"}
        };
        // 写完要关闭文件，否则其他地方无法读
        CHECK(writer.write_all(wdata));
        writer.close();

        CsvReader reader(fname, ' ');
        CsvData rdata;
        CHECK(reader.read_all(rdata));
        CHECK_EQ(wdata, rdata);
        reader.close();
        CHECK(FileSys::delete_file(fname));
    }

    TEST_CASE("test_xcsv::write_row_and_read_row")
    {
        std::string fname("output.csv");
        CsvWriter writer(fname);
        CsvRow wrow1 = {"Alice", "25", "New York"};
        CsvRow wrow2 = {"Bob", "30", "London"};

        CHECK(writer.write_row(wrow1));
        CHECK(writer.write_row(wrow2));
        writer.close();

        CsvReader reader(fname);
        CsvRow rrow1;
        CsvRow rrow2;
        CHECK(reader.read_row(rrow1));
        CHECK(reader.read_row(rrow2));
        reader.close();

        CHECK_EQ(wrow1, rrow1);
        CHECK_EQ(wrow2, rrow2);
        CHECK(FileSys::delete_file(fname));
    }
}
