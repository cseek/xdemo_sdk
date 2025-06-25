/*
 * @Author: aurson jassimxiong@gmail.com
 * @Date: 2025-06-25 23:30:09
 * @LastEditors: aurson jassimxiong@gmail.com
 * @LastEditTime: 2025-06-26 01:31:46
 * @Description: 对于 CSVReader, 即使空格的数量不一致，也不会影响分割
 * Copyright (c) 2025 by Aurson, All Rights Reserved.
 */
#ifndef __XCSV_H__
#define __XCSV_H__

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using CsvRow = std::vector<std::string>;
using CsvData = std::vector<CsvRow>;

class CsvReader
{
public:
    explicit CsvReader(const std::string &fname, char delimiter = ',')
        : m_delimiter(delimiter)
    {
        m_ifs = std::move(std::ifstream(fname));
    }

    bool read_all(CsvData &data)
    {
        if (!m_ifs.is_open())
        {
            return false;
        }

        std::string line;
        while (std::getline(m_ifs, line))
        {
            data.push_back(parse_line(line));
        }

        return true;
    }

    bool read_row(CsvRow &data)
    {
        if (!m_ifs.is_open())
        {
            return false;
        }

        std::string line;
        std::getline(m_ifs, line);
        data = parse_line(line);

        return true;
    }

    void close()
    {
        m_ifs.close();
    }

private:
    CsvRow parse_line(const std::string &line)
    {
        CsvRow tokens;
        std::string token;
        std::istringstream token_stream(line);

        while (std::getline(token_stream, token, m_delimiter))
        {
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);

            if (!token.empty())
            {
                tokens.push_back(token);
            }
        }

        return tokens;
    }

private:
    char m_delimiter;
    std::ifstream m_ifs;
};

class CsvWriter
{
public:
    explicit CsvWriter(const std::string &fname, char delimiter = ',')
        : m_delimiter(delimiter)
    {
        m_ofs = std::move(std::ofstream(fname));
    }

    bool write_all(const CsvData &data)
    {
        if (!m_ofs.is_open())
        {
            return false;
        }

        for (const auto &row : data)
        {
            for (size_t i = 0; i < row.size(); ++i)
            {
                m_ofs << row[i];
                if (i != row.size() - 1)
                {
                    m_ofs << m_delimiter;
                }
            }
            m_ofs << "\n";
        }
        return true;
    }

    bool write_row(const CsvRow &data)
    {
        if (!m_ofs.is_open())
        {
            return false;
        }

        // 移动到文件末尾以追加内容
        m_ofs.seekp(0, std::ios_base::end);

        for (size_t i = 0; i < data.size(); ++i)
        {
            m_ofs << data[i];
            if (i != data.size() - 1)
            {
                m_ofs << m_delimiter;
            }
        }
        m_ofs << "\n";

        return true;
    }

    void close()
    {
        m_ofs.close();
    }

private:
    char m_delimiter;
    std::ofstream m_ofs;
};

#endif // __XCSV_H__