//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_STUDENTPARSER_H
#define ABSCHLUSSAUFGABE_STUDENTPARSER_H

#include <unordered_map>
#include "CSVParser.h"
#include "ExamParser.h"
#include "Time.h"

class StudentParser : CSVParser {
public:
    StudentParser(std::string fileName);
    virtual ~StudentParser();
    std::unordered_map<std::string, std::unordered_map<int, std::unordered_map<std::string, ExamParser::Exam>>>
    getStudents();
    bool testTime(Time t,  std::string fieldOfStudy, int matrikelNumber);
    void removeExam(std::string fieldOfStudy, int matrikelNumber, std::string key);
private:
    std::unordered_map<std::string,std::unordered_map<int, std::unordered_map<std::string, ExamParser::Exam>>> students;
};
#endif //ABSCHLUSSAUFGABE_STUDENTPARSER_H
