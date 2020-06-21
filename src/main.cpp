#include <iostream>
#include "../header/CSVParser.h"
#include "../header/CSVWriter.h"
#include "../header/ExamParser.h"
#include "../header/StudentParser.h"
#include "../header/RoomParser.h"

#include <string>

using namespace std;

int main() {
    //example use of CSVParser:
    string fileName = "../InputData/Raumliste.csv";
    CSVParser csvParser(fileName, CSVParser::separator::comma);

    vector<string> out;
    out = csvParser.getColumn(2);

    for (const auto& elem : out){
        cout << elem << " || ";
    }
}
