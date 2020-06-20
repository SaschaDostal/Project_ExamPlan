#include <iostream>
#include "../header/CSVParser.h"
#include "../header/CSVWriter.h"
#include "../header/ExamParser.h"
#include "../header/StudentParser.h"
#include "../header/RoomParser.h"

using namespace std;

int main() {
    //example use of CSVParser: always add 1 to maxCellLength for String ending 0
    CSVParser csvParser("../InputData/Raumliste.csv", CSVParser::separator::comma, 54, 10, 16);

    char *out = (char *) malloc(25);
    csvParser.getCell(3, 7, &out);
    cout << out << endl;

}
