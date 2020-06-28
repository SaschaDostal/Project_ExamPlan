//
// Created by felix on 20/06/2020.
//

#include "../header/CSVWriter.h"
#include <iostream>

using namespace std;

CSVWriter::CSVWriter(vector<ExamParser::Exam> examsToWrite) {
    writeExams(examsToWrite, separator::semicolon);
}

CSVWriter::CSVWriter(vector<ExamParser::Exam> examsToWrite, separator separator) {
    writeExams(examsToWrite, separator);
}

CSVWriter::~CSVWriter() = default;

void CSVWriter::writeExams(std::vector<ExamParser::Exam> exams, separator separator) {
    file.open("../OutputData/Geplante_Prüfungen.csv");
    if(file.is_open()){
        file << "stg;vert;pversion;pnr;pdtxt;ppruefer;pdauer;pform;psem;angeboten;r\xE4ume;tag;uhrzeit" << endl;
        for (int i=0; i<exams.size(); i++){
            if(exams[i].planned) writeLine(exams[i]);
        }
        file.close();
    } else {
        file.close();
        cerr << "error opening file" << endl;
    }
}

void CSVWriter::writeLine(ExamParser::Exam ex){
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
            << ex.examTime
            << endl;
}