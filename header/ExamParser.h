//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_EXAMPARSER_H
#define ABSCHLUSSAUFGABE_EXAMPARSER_H

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
        //std::string getKey() { return fieldOfStudy.append(std::to_string(examVersion)).append(std::to_string(examNumber));}
    };
    ExamParser(std::string fileName);
    virtual ~ExamParser();
    std::vector<Exam>& getExams();
private:
    std::vector<Exam> exams;
};

class ExamVector : public std::vector<ExamParser::Exam>{
public:
    std::mutex examMutex;
};

#endif //ABSCHLUSSAUFGABE_EXAMPARSER_H
