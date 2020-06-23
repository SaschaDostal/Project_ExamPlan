//
// Created by felix on 20/06/2020.
//

#include "../header/ExamParser.h"
#include "../header/CSVParser.h"
using namespace std;

ExamParser::ExamParser(string fileName) : CSVParser(fileName, separator::semicolon){
    tableData tmp = getData();
    for(int i=1; i<tmp.size(); i++){
        Exam exam = {};
        vector<string> examData = getRow(i);
        {
            exam.fieldOfStudy = examData.at(0);
            exam.distributor = examData.at(1);
            exam.examVersion = stoi(examData.at(2));
            exam.examNumber = stoi(examData.at(3));
            exam.examName = examData.at(4);
            exam.examiner = examData.at(5);
            exam.examLength = stoi(examData.at(9));
            exam.typeOfExam = examData.at(10);
            exam.examSemester = stoi(examData.at(11));
            exam.provided = (examData.at(12) == "J");
            exam.examTime.duration = exam.examLength;
        }
        exams.push_back(exam);
    }
}

ExamParser::~ExamParser() = default;

vector<ExamParser::Exam> ExamParser::getExams() {
    return exams;
}