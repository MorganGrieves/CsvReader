#ifndef CSVTABLE_H
#define CSVTABLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <string_view>
#include <sstream>
#include <regex>
#include <algorithm>
#include <unordered_map>
#include <utility>

class CsvTable
{
    enum class Error
    {
        NoError,

        HeaderExistError,
        HeaderEmptyError,
        HeaderFormatError,

        FailedFileError,

        InvalidValueError,

        FitSizeError,

        LabelEmptyError,
        LabelExistError,
        LabelFormatError,

        InvalidError,
    };

public:
    CsvTable(const std::string_view filename);
    void printTable() const;
    bool good() const;
    std::vector<std::string> headers() const;
    std::vector<std::string> rowLabels() const;

    std::unordered_map<std::string, std::string>& operator[] (const std::string key);
    const std::unordered_map<std::string, std::string>& operator[] (const std::string key) const;

    ~CsvTable();

private:
    void errorHandle(const Error &err, unsigned row = 0, unsigned column = 0);

    void headerParse(std::ifstream &file);
    void bodyParse(std::ifstream &file);

    bool headerExist(const std::string &header) const;
    Error checkHeader(const std::string &header) const;

    bool rowLabelExist(const std::string &label) const;
    Error checkRowLabel(const std::string &label) const;

    Error checkValue(const std::string &value) const;

    void insert(std::string rowName, std::unordered_map<std::string, std::string> &values);

private:
    std::string mFilepath = "";
    std::vector<std::string> mTableHeaders;
    std::vector<std::string> mTableRowLabels;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> mTableData;
    Error mErrorFlag = Error::NoError;
};

#endif // CSVTABLE_H
