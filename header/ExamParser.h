//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_EXAMPARSER_H
#define ABSCHLUSSAUFGABE_EXAMPARSER_H

#include <unordered_map>
#include "CSVParser.h"
#include "Time.h"
#include <mutex>

class ExamParser : CSVParser {
public:
    struct Exam{
        std::string fieldOfStudy;
        std::string distributor;
        int examVersion;
        int examNumber;
        std::string examName;
        std::string examiner;
        Time examTime;
        int examLength;
        std::string typeOfExam;
        int examSemester;
        bool provided;
        bool planned;
        std::string rooms;
        std::string getKey() {return fieldOfStudy + std::to_string(examVersion) + std::to_string(examNumber);}
    };
    ExamParser(std::string fileName);
    virtual ~ExamParser();
    std::unordered_map<std::string, Exam>& getExams();
private:
    std::unordered_map<std::string, Exam> exams;
};

#endif //ABSCHLUSSAUFGABE_EXAMPARSER_H
