//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_EXAMPARSER_H
#define ABSCHLUSSAUFGABE_EXAMPARSER_H

#include "CSVParser.h"

class ExamParser : CSVParser {
public:
    struct Exam{
        std::string fieldOfStudy;
        int distributorNr;
        int examVersion;
        int examNumber;
        std::string examName;
        std::string examiner;

        int examLength;
        std::string typeOfExam;
        int examSemester;
        bool provided;
    };
    ExamParser();
    virtual ~ExamParser();
    Exam* getExams();
private:
    Exam *exams;
    int examCount;
};

#endif //ABSCHLUSSAUFGABE_EXAMPARSER_H
