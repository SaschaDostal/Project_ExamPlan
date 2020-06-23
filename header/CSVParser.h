//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_CSVPARSER_H
#define ABSCHLUSSAUFGABE_CSVPARSER_H

#include <fstream>
#include <vector>

typedef std::vector<std::vector<std::string>> tableData;

class CSVParser {
public:
    enum class separator{ comma, semicolon};
    CSVParser(std::string fileName);
    CSVParser(std::string fileName, separator separator);
    virtual ~CSVParser();

protected:
    void parse();
    tableData getData();
    std::string getCell(int row, int column);
    std::vector<std::string> getRow(int row);
    std::vector<std::string> getColumn(int column);

private:
    separator mySeparator;
    std::string fileName;
    std::ifstream file;
    tableData tData;
};

#endif //ABSCHLUSSAUFGABE_CSVPARSER_H