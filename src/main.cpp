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

    //example use of examParser:
    string examFile = "../InputData/Angebotene_Prüfungen_KL.csv";
    ExamParser examParser(examFile);

    //example use of examParser get Function:
    vector<ExamParser::Exam> exams = examParser.getExams();
    cout << exams.at(23).examName << endl;

    /*example use of CSVParser get Function:
    vector<string> out;
    out = csvParser.getColumn(2);

    for (const auto& elem : out){
        cout << elem << " || ";
    }
     */
}
