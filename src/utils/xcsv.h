/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-25 23:30:09
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-25 23:46:04
 * @Description:
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __XCSV_H__
#define __XCSV_H__

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

using CsvData = std::vector<std::vector<std::string>>;
class CSVReader
{
public:
    explicit CSVReader(const std::string &filename, char delimiter = ',')
        : m_filename(filename),
          m_delimiter(delimiter) {}

    bool read(CsvData &data)
    {
        std::ifstream ifs(m_filename);
        if (!ifs.is_open())
        {
            return false;
        }

        std::string line;

        while (std::getline(ifs, line))
        {
            data.push_back(parse_line(line));
        }

        return true;
    }

private:
    std::vector<std::string> parse_line(const std::string &line)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(line);

        while (std::getline(tokenStream, token, m_delimiter))
        {
            tokens.push_back(token);
        }

        return tokens;
    }

private:
    std::string m_filename;
    char m_delimiter;
};

class CSVWriter
{
public:
    explicit CSVWriter(const std::string &filename, char delimiter = ',')
        : m_filename(filename),
          m_delimiter(delimiter) {}

    bool write(const CsvData &data)
    {
        std::ofstream ofs(m_filename);
        if (!ofs.is_open())
        {
            return false;
        }

        for (const auto &row : data)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                ofs << row[i];
                if (i != row.size() - 1)
                {
                    ofs << m_delimiter;
                }
            }
            ofs << "\n";
        }
        return true;
    }

private:
    std::string m_filename;
    char m_delimiter;
};

#endif // __XCSV_H__