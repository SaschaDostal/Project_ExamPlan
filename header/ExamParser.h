//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_EXAMPARSER_H
#define ABSCHLUSSAUFGABE_EXAMPARSER_H

#include "CSVParser.h"

class ExamParser : CSVParser {
public:
    struct Exam{
        char *fieldOfStudy;
        int distributorNr;
        int examVersion;
        int examNumber;
        char *examName;
        char *examiner;

        int examLength;
        char *typeOfExam;
        int examSemester;
        bool provided;
    };
    ExamParser();
    virtual ~ExamParser();
    void getExams(Exam *exams, int &examCount);
private:
    Exam *exams;
    int examCount;
};

#endif //ABSCHLUSSAUFGABE_EXAMPARSER_H
