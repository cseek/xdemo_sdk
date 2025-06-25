/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-25 23:34:26
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-25 23:56:08
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
    TEST_CASE("test_xcsv::write_and_read")
    {
        std::string fname("output.csv");
        CSVWriter writer(fname);
        CsvData wdata = {
            {"Name", "Age", "City"},
            {"Alice", "25", "New York"},
            {"Bob", "30", "London"}
        };
        CHECK(writer.write(wdata));
        CSVReader reader(fname);
        CsvData rdata;
        CHECK(reader.read(rdata));
        CHECK(wdata==rdata);
        CHECK(FileSys::delete_file(fname));
    }
}