//
// Created by felix on 20/06/2020.
//

#include "../header/StudentParser.h"
#include "../header/CSVParser.h"
using namespace std;

StudentParser::StudentParser(string fileName) : CSVParser(fileName, separator::semicolon){
    tableData tmp = getData();
    for(int i=1; i<tmp.size(); i++){
        Student student = {};
        //hier fehlt noch was
        vector<string> examData = getRow(i);
        {
            student.matrikelNumber = stoi(examData.at(0));
        }
        students.push_back(student);
    }
}

StudentParser::~StudentParser() = default;

vector<StudentParser::Student> StudentParser::getStudents() {
    return students;
}