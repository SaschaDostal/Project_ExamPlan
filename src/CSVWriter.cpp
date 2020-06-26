//
// Created by felix on 20/06/2020.
//

#include "../header/CSVWriter.h"
#include <iostream>

using namespace std;

CSVWriter::CSVWriter(vector<ExamParser::Exam> examsToWrite) {
    writeExams(examsToWrite, separator::semicolon);
}

CSVWriter::CSVWriter(vector<ExamParser::Exam> examsToWrite, separator separator) {
    writeExams(examsToWrite, separator);
}

CSVWriter::~CSVWriter() = default;

void CSVWriter::writeExams(std::vector<ExamParser::Exam> exams, separator separator) {
    file.open("../OutputData/Geplante_Prüfungen.csv");
    if(file.is_open()){

    } else {
        file.close();
        cerr << "error opening file" << endl;
    }
}