//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_CSVPARSER_H
#define ABSCHLUSSAUFGABE_CSVPARSER_H

#include <fstream>

typedef char*** tableData;

class CSVParser {
public:
    enum class separator{ comma, semicolon};
    CSVParser(char *fileName);
    CSVParser(char *fileName, separator separator, int rowCount, int columnCount, int maxCellLength);
    CSVParser(char *fileName, separator separator);
    virtual ~CSVParser();

//protected:
    void parse();
    void getData(tableData *tableData);
    void getCell(int row, int column, char* content[]);
    void getRow(int row, char*** content);
    void getColumn(int column, char*** content);

private:
    separator separator;
    char *fileName;
    std::ifstream file;
    int rowCount;
    int columnCount;
    int maxCellLength;
    tableData tData;
};

#endif //ABSCHLUSSAUFGABE_CSVPARSER_H