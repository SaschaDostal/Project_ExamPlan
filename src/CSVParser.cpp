//
// Created by felix on 20/06/2020.
//

#include "../header/CSVParser.h"
#include <iostream>
#include <fstream>

using namespace std;

CSVParser::CSVParser(char *fileName) : fileName(fileName), separator(separator::semicolon), rowCount(60), columnCount(60), maxCellLength(60){
    parse();
}

CSVParser::CSVParser (char *fileName, enum separator separator, int rowCount, int columnCount, int maxCellLength) : fileName(fileName), separator(separator), rowCount(rowCount), columnCount(columnCount), maxCellLength(maxCellLength) {
    parse();
}

CSVParser::CSVParser(char *fileName, enum separator separator) : fileName(fileName), separator(separator), rowCount(60), columnCount(60), maxCellLength(60){
    parse();
}

void CSVParser::parse(){
    tData = (char***) malloc(rowCount * sizeof(char **));
    for (int i=0; i<rowCount; i++){
        tData[i] = (char **) malloc(columnCount * sizeof(char *));
        for (int b=0; b<maxCellLength; b++){
            tData[i][b] = (char *) malloc(maxCellLength * sizeof(char));
        }
    }
    int nsize = columnCount*maxCellLength;
    char data[nsize] = {0};
    file.open(fileName);
    if(file.is_open()){
        int row = 0;
        while (file.getline(data,nsize)){
            int column = 0;
            int tmpI = 0;
            char tmp[maxCellLength] = {0};
            for(int i=0; i<nsize; i++){
                if(data[i] == 0){
                    break;
                }
                if(data[i] == ((separator == separator::comma)? ',':';')){
                    tmp[tmpI] = 0;
                    tData[row][column] = tmp;
                    tmpI = 0;
                    for(int c=0; c<maxCellLength; c++)
                        tmp[c] = 0;
                } else {
                    tmp[tmpI++] = data[i];
                }
            }
        }
    } else {
        cout << "error opening file" << '\n';
    }
    file.close();
}

void CSVParser::getData(tableData *tableData) {
    tableData = &tData;
}

void CSVParser::getCell(int row, int column, char* content[]) {
    content = &tData[row][column];
}

void CSVParser::getColumn(int column, char ***content) {
    char tmpContent[rowCount][maxCellLength];
    for(int i=0; i<rowCount; i++) {
        //getCell(i, column, content[i]);
    }
}

void CSVParser::getRow(int row, char ***content) {
    content = &tData[row];
}

CSVParser::~CSVParser() = default;
