//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_STUDENTPARSER_H
#define ABSCHLUSSAUFGABE_STUDENTPARSER_H

#include "CSVParser.h"
#include "ExamParser.h"

class StudentParser : CSVParser {
public:
    struct Student {
        int matrikelNumber;
        ExamParser::Exam exam;
    };
    StudentParser();
    virtual ~StudentParser();
    void getStudents(Student *students, int &studentCount);

private:
    Student *students;
    int studentCount;
};
#endif //ABSCHLUSSAUFGABE_STUDENTPARSER_H
