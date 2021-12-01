#include "csv_table.h"

CsvTable::CsvTable(const std::string_view filename)
{
    std::ifstream csvFile(filename.data());

    if (!csvFile)
    {
        errorHandle(Error::FailedFileError);
        return;
    }

    unsigned currentRow = 0;
    unsigned currentColumn = 0;

    // headers processing
    std::string line = "";
    std::getline(csvFile, line);
    std::istringstream headerStream(line);

    std::string emptyCell;
    std::getline(headerStream, line, ',');
    if (!line.empty())
        std::cout << "warning: csv_table: первый заголовок не пуст" << std::endl;

    for (std::string header; std::getline(headerStream, header, ','); )
    {
        Error err = checkHeader(header);

        if (err == Error::NoError)
            mTableHeaders.push_back(header);
        else
        {
            errorHandle(err, 0, currentColumn++);
            csvFile.close();
            return;
        }
    }

    // data processing
    currentRow = 0;
    currentColumn = 0;

    while (std::getline(csvFile, line))
    {
        std::string rowName = "";
        std::istringstream valueStream(line);
        std::getline(valueStream, rowName, ',');

        Error err = checkRowLabel(rowName);
        if (err == Error::NoError)
            mTableRowLabels.push_back(rowName);
        else
        {
            errorHandle(err, currentRow, 0);
            csvFile.close();
            return;
        }

        std::unordered_map<std::string, std::string> values;

        for (std::string value; std::getline(valueStream, value, ','); )
        {
            Error err = checkValue(value);

            if (err == Error::NoError)
                values[mTableHeaders.at(currentColumn++)] = value;
            else
            {
                errorHandle(err, currentRow, currentColumn);
                csvFile.close();
                return;
            }
        }

        if (values.size() != mTableHeaders.size())
        {
            errorHandle(Error::FitSizeError, currentRow, currentColumn);
            csvFile.close();
            return;
        }

        insert(rowName, values);

        currentColumn = 0;
        currentRow++;
    }
    csvFile.close();
}

void CsvTable::printTable() const
{  
    for (auto header = mTableHeaders.begin(); header != mTableHeaders.end(); ++header)
    {
        std::cout << "\t" << *header;
    }
    std::cout << std::endl;

    for (auto row = mTableData.cbegin(); row != mTableData.cend(); ++row)
    {
        std::cout << row->first;

        for (auto header = mTableHeaders.cbegin(); header != mTableHeaders.cend(); ++header)
            std::cout << "\t" << row->second.at(*header);

        std::cout << std::endl;
    }
}

bool CsvTable::good() const
{
    return (mErrorFlag == Error::NoError) ? true : false;
}

std::vector<std::string> CsvTable::headers() const
{
    return mTableHeaders;
}

std::vector<std::string> CsvTable::rowLabels() const
{
    return mTableRowLabels;
}

std::unordered_map<std::string, std::string>&  CsvTable::operator[] (const std::string key)
{
    return mTableData[key];
}

const std::unordered_map<std::string, std::string>&  CsvTable::operator[] (const std::string key) const
{
    return mTableData.at(key);
}

CsvTable::~CsvTable()
{}

void CsvTable::errorHandle(const CsvTable::Error &err,
                           unsigned row,
                           unsigned column)
{
    switch (err)
    {
    //Header Handling
    case Error::HeaderEmptyError:
        std::cerr << "CsvTable: [ _, " << column << "]"
                     " Заголовок пуст" << std::endl;
        break;

    case Error::HeaderExistError:
        std::cerr << "CsvTable: [ _, " << column << "]"
                  << " Заголовок уже существует";
        break;

    case Error::HeaderFormatError:
        std::cerr << "CsvTable: [ _, " << column << "]"
                  << " Заголовок не соответствует формату заголовков."
                     " Заголовок должен иметь латинские буквы и/или символ подчеркивания" << std::endl;
        break;

    //other errors
    case Error::FailedFileError:
        std::cerr << "CsvTable: не удалось открыть файл таблицы " << std::endl;
        break;

    case Error::InvalidValueError:
        std::cerr << "CsvTable: [" << row << ", " << column << "]"
                  << " Invalid value" << std::endl;
        break;

    case Error::FitSizeError:
        std::cerr << "CsvTable: [" << row << ", " << column << "]"
                  << " не соответствие размеров колонок со строками" << std::endl;
        break;

    //row label handle
    case Error::LabelEmptyError:
        std::cerr << "CsvTable: [" << row << ", _ ]"
                  << " Ключ пустой" << std::endl;
        break;

    case Error::LabelExistError:
        std::cerr << "CsvTable: [" << row << ", _ ]"
                  << " Ключ существует" << std::endl;
        break;

    case Error::LabelFormatError:
        std::cerr << "CsvTable: [" << row << ", _ ]"
                  << " Ключ не соответствует формату."
                  << " Ключ должен быть целочисленным" << std::endl;
        break;

    //No Error Handle
    default:
        std::cerr << "CsvTable: Добавьте обработчик ошибки" << std::endl;
    }

    mErrorFlag = err;
}


void CsvTable::headerParse(std::ifstream &file)
{
////!!!!!!
}

void CsvTable::bodyParse(std::ifstream &file)
{
////!!!!!!
}

bool CsvTable::headerExist(const std::string &header) const
{
    return (std::find(mTableHeaders.cbegin(), mTableHeaders.cend(), header) != mTableHeaders.cend());
}

CsvTable::Error CsvTable::checkHeader(const std::string &header) const
{
    if (header.empty())
        return Error::HeaderEmptyError;

    if (headerExist(header))
        return Error::HeaderExistError;

    std::regex headerPat (R"(^[a-zA-Z_]+$)");
    if (!std::regex_match(header, headerPat))
         return Error::HeaderFormatError;

    return Error::NoError;
}

bool CsvTable::rowLabelExist(const std::string &label) const
{
    ////!!!!!!!!
    return false;
}

CsvTable::Error CsvTable::checkRowLabel(const std::string &label) const
{
    if (label.empty())
        return Error::LabelEmptyError;

    if (rowLabelExist(label))
        return Error::LabelExistError;

    std::regex labelPat (R"(^-?(0|[1-9]\d*)$)");
    if (!std::regex_match(label, labelPat))
         return Error::LabelFormatError;

    return Error::NoError;
}

CsvTable::Error CsvTable::checkValue(const std::string &value) const
{
    if (value.empty())
        return Error::InvalidValueError;

    return Error::NoError;
}

void CsvTable::insert(std::string rowName, std::unordered_map<std::string, std::string> &values)
{
    mTableData[rowName] = values;
}
