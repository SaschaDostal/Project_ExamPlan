//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_STUDENTPARSER_H
#define ABSCHLUSSAUFGABE_STUDENTPARSER_H

#include "CSVParser.h"
#include "ExamParser.h"
#include "Time.h"

class StudentParser : CSVParser {
public:
    struct Student {
        int matrikelNumber;
        std::string fieldOfStudy;
        std::vector<ExamParser::Exam> exams;
    };
    StudentParser(std::string fileName,  std::vector<ExamParser::Exam> allExams);
    virtual ~StudentParser();
    std::vector<Student>& getStudents();
    bool testTime(Time t,  StudentParser::Student s);
private:
    std::vector<Student> students;
    int studentCount;
};
#endif //ABSCHLUSSAUFGABE_STUDENTPARSER_H
