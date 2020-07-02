//
// Created by felix on 20/06/2020.
//

#include "../header/CSVWriter.h"
#include <iostream>

using namespace std;

CSVWriter::CSVWriter(const std::vector<std::pair<std::string, ExamParser::Exam>>& examsToWrite, const unordered_map<string, unordered_map<int, unordered_map<string, ExamParser::Exam>>>& studentsToWrite) {
    writeExams(examsToWrite, studentsToWrite, separator::semicolon);
}

CSVWriter::CSVWriter(const std::vector<std::pair<std::string, ExamParser::Exam>>& examsToWrite, const unordered_map<string, unordered_map<int, unordered_map<string, ExamParser::Exam>>>& studentsToWrite, separator separator) {
    writeExams(examsToWrite, studentsToWrite, separator);
}

CSVWriter::~CSVWriter() = default;

void CSVWriter::writeExams(const std::vector<std::pair<std::string, ExamParser::Exam>>& exams, const unordered_map<string, unordered_map<int, unordered_map<string, ExamParser::Exam>>>& studentsToWrite, separator separator) {
    file.open("../OutputData/Geplante_Prüfungen.csv");
    if(file.is_open()){
        file << "stg;vert;pversion;pnr;pdtxt;ppruefer;pdauer;pform;psem;angeboten;(r\xE4ume|aufsicht);rkapazit\xE4t;anzstudenten;restplatz;tag;uhrzeit" << endl;
        for (auto& exam : exams){
            if(exam.second.planned) writeLine(exam.second);
        }
        file.close();
    } else {
        file.close();
        cerr << "error opening file" << endl;
    }
    file.open("../OutputData/Ungeplante_Prüfungen.csv");
    if(file.is_open()){
        file << "stg;vert;pversion;pnr;pdtxt;ppruefer;pdauer;pform;psem;angeboten;(r\xE4ume|aufsicht);rkapazit\xE4t;anzstudenten;restplatz;tag;uhrzeit" << endl;
        for (auto& exam : exams){
            if(!exam.second.planned) writeLine(exam.second);
        }
        file.close();
    } else {
        file.close();
        cerr << "error opening file" << endl;
    }

    file.open("../OutputData/Studenten_Übersicht.csv");
    if(file.is_open()){
        file << "mtknr;tag;uhrzeit" << endl;
        for (auto& fieldOfStudy : studentsToWrite) {
            for(auto& stud : fieldOfStudy.second) {
                writeLine(stud);
            }
        }
        file.close();
    } else {
        file.close();
        cerr << "error opening file" << endl;
    }

}

void CSVWriter::writeLine(const ExamParser::Exam& ex){
    file    << ex.fieldOfStudy << ";"
            << ex.distributor << ";"
            << ex.examVersion << ";"
            << ex.examNumber << ";"
            << ex.examName << ";"
            << ex.examiner << ";"
            << ex.examLength << ";"
            << ex.typeOfExam << ";"
            << ex.examSemester << ";"
            << (ex.provided ? "J" : "N") << ";"
            << ex.rooms << ";"
            << ex.roomCapacity << ";"
            << ex.numStudents << ";"
            //<< ex.freeSpace << ";"
            << ex.examTime << ";"
            << endl;
}

void CSVWriter::writeLine(const pair<int, unordered_map<string, ExamParser::Exam>>& stud){
    int a = 0;
    a++;
    if(stud.first != 0) {
        file << stud.first << ";";
        for (auto &exam : stud.second) {
            file << exam.second.examTime << ";";
        }
        file << endl;
    }
    cout << "Number of students(may include doubles due to exams written in different fields of study): " << a << endl;
}
