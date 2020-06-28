//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_CSVWRITER_H
#define ABSCHLUSSAUFGABE_CSVWRITER_H

#include <cstdio>
#include <vector>
#include "ExamParser.h"

class CSVWriter {
public:
    enum class separator{ comma, semicolon};
    CSVWriter(std::vector<std::pair<std::string, ExamParser::Exam>> examsToWrite);
    CSVWriter(std::vector<std::pair<std::string, ExamParser::Exam>> examsToWrite, separator separator);
    virtual ~CSVWriter();

private:
    void writeLine(ExamParser::Exam ex);
    void writeExams(std::vector<std::pair<std::string, ExamParser::Exam>> exams, separator separator);
    std::ofstream file;
};

#endif //ABSCHLUSSAUFGABE_CSVWRITER_H
