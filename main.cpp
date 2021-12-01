#include <iostream>

#include "csv_table.h"


void calculateExpressions(CsvTable &table)
{
    std::vector<std::string> headers = table.headers();
    std::vector<std::string> rowLabels = table.rowLabels();

    std::regex numberPat (R"(^-?(0|[1-9]\d*)$)");
    std::smatch numberMatches;

    for (auto rowIt = rowLabels.begin(); rowIt != rowLabels.end(); ++rowIt)
    {
        for (auto headerIt = headers.begin(); headerIt != headers.end(); ++headerIt)
        {
            auto header = *headerIt;
            auto rowLabel = *rowIt;

            if (std::regex_match(table[rowLabel][header], numberMatches, numberPat))
                continue;

            std::regex expPat(R"(^\s*=\s*((?:-?(?:0|[1-9]\d*))|(?:[a-zA-Z_]+[1-9]\d*))\s*([+|-|*|\/])\s*((?:-?(?:0|[1-9]\d*))|(?:[a-zA-Z_]+[1-9]\d*))\s*$)");
            std::smatch expMatches;

            if (std::regex_match(table[rowLabel][header], expMatches, expPat))
            {
                for (auto it = expMatches.begin(); it != expMatches.end(); ++it)
                    std::cout << *it << std::endl;

                std::string arg1 = expMatches[1];
                char op = expMatches.str(2)[0];
                std::string arg2 = expMatches[3];

                std::regex argPat(R"(^(?:(-?(?:0|[1-9]\d*))|(?:([a-zA-Z_]+)([1-9]\d*)))$)");
                std::smatch argMatches;

                int number1 = 0;
                if (std::regex_match(arg1, argMatches, argPat))
                {
                    if (argMatches.size() == 4)
                    {
                        std::string header = argMatches[2];
                        std::string rowLabel = argMatches[3];
                        number1 = std::stoi(table[rowLabel][header]);
                    }
                    else
                        number1 = std::stoi(arg1);
                }

                int number2 = 0;
                if (std::regex_match(arg2, argMatches, argPat))
                {
                    if (argMatches.size() == 4)
                    {
                        std::string header = argMatches[2];
                        std::string rowLabel = argMatches[3];
                        number2 = std::stoi(table[rowLabel][header]);
                    }
                    else
                        number2 = std::stoi(arg2);
                }

                switch (op)
                {
                case '+':
                    table[rowLabel][header] = std::to_string(number1 + number2);
                    break;

                case '-':
                    table[rowLabel][header] = std::to_string(number1 - number2);
                    break;

                case '*':
                    table[rowLabel][header] = std::to_string(number1 * number2);
                    break;

                case '/':
                    table[rowLabel][header] = std::to_string(number1 / number2); // check 0
                    break;
                }
            }
            else
                std::cout << "what's wrong with " << table[rowLabel][header] << std::endl;
        }
    }
}

int main(int argc, char * argv[])
{
//    if (argc < 2)
//    {
//        std::cerr << "Мало аргументов" << std::endl;
//        return 1;
//    }

    CsvTable table("/home/morgangrieves/QtProjects/CsvReader/data/samples.csv");

    if (table.good())
    {
        calculateExpressions(table);
        table.printTable();
    }

    return 0;
}
