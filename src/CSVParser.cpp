//
// Created by felix on 20/06/2020.
//

#include "../header/CSVParser.h"
#include <iostream>
#include <fstream>

using namespace std;

CSVParser::CSVParser(string fileName) : fileName(fileName), separator(separator::semicolon){
    parse();
}

CSVParser::CSVParser(string fileName, enum separator separator) : fileName(fileName), separator(separator){
    parse();
}

void CSVParser::parse(){
    file.open(fileName);
    if(file.is_open()){
        string line;
        while (getline(file,line)){
            vector<string> tmp;
            for (int separatorIndex = 0; separatorIndex < line.length(); separatorIndex = line.find((separator == separator::comma)? ',':';', separatorIndex+1)){
                tmp.push_back(line.substr(separatorIndex+1, line.find((separator == separator::comma)? ',':';', separatorIndex+1)-separatorIndex));
            }
            tData.push_back(tmp);
        }
    } else {
        cout << "error opening file" << '\n';
    }
    file.close();
}

tableData CSVParser::getData() {
    return tData;
}

string CSVParser::getCell(int row, int column) {
    return tData[row][column];
}

vector<string> CSVParser::getColumn(int column) {
    vector<string> tmp;
    for(int i=0; i < tData.size(); i++) {
        tmp.push_back(tData.at(i).at(column));
    }
    return tmp;
}

vector<string> CSVParser::getRow(int row) {
    return tData[row];
}

CSVParser::~CSVParser() = default;
