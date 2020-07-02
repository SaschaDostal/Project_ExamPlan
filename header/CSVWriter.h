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
    CSVWriter(const std::vector<std::pair<std::string, ExamParser::Exam>>& examsToWrite, const std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<std::string, ExamParser::Exam>>>& studentsToWrite);
    CSVWriter(const std::vector<std::pair<std::string, ExamParser::Exam>>& examsToWrite, const std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<std::string, ExamParser::Exam>>>& studentsToWrite, separator separator);
    virtual ~CSVWriter();

private:
    void writeLine(const ExamParser::Exam& ex);
    void writeLine(const std::pair<int, std::unordered_map<std::string, ExamParser::Exam>>& stud);
    void writeExams(const std::vector<std::pair<std::string, ExamParser::Exam>>& exams, const std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<std::string, ExamParser::Exam>>>& studentsToWrite, separator separator);
    std::ofstream file;
};

#endif //ABSCHLUSSAUFGABE_CSVWRITER_H
