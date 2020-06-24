//
// Created by felix on 20/06/2020.
//

#include <iostream>
#include "../header/StudentParser.h"
#include "../header/CSVParser.h"
#include "../header/ExamParser.h"
using namespace std;

StudentParser::StudentParser(string fileName, std::vector<ExamParser::Exam> allExams) : CSVParser(fileName, separator::semicolon){
    tableData tmp = getData();
    for(int i=1; i<tmp.size(); i++){
        Student student = {};
        vector<string> studentData = getRow(i);
        bool studExists = false;
        for(Student& stud : students){
            if(stoi(studentData.at(0)) == stud.matrikelNumber){
                studExists = true;
            }
        }
        // Wenn Student nicht existiert, setze Matrikelnummer, füge Prüfung zu Prüfungen hinzu
        // und füge Student zum Student-Vector hinzu
        if(!studExists){
            student.matrikelNumber = stoi(studentData.at(0));
            student.fieldOfStudy = studentData.at(1);
            bool examExists = false;
            for(ExamParser::Exam& e : allExams){
                if(stoi(studentData.at(3)) == e.examNumber  && stoi(studentData.at(2)) == e.examVersion
                    && (e.fieldOfStudy.compare(studentData.at(1)) == 0)){
                    student.exams.push_back(e);
                    examExists = true;
                }
            }
            if(examExists){
                students.push_back(student);
            } else {
                cout << "registration ignored: Exam with pnumber " << stoi(studentData.at(3)) << ", version " << studentData.at(2)
                     << " stg " << studentData.at(1) << " does not exist." << endl;
            }
        // Wenn Student existiert, suche Student in Student-Vector und füge Prüfung hinzu
        } else {
            for(Student& stud : students){
                if(stoi(studentData.at(0)) == stud.matrikelNumber){
                    bool examExists = false;
                    for(ExamParser::Exam& e : allExams){
                        if((stoi(studentData.at(3)) == e.examNumber) && (stoi(studentData.at(2)) == e.examVersion)
                            && (e.fieldOfStudy.compare(studentData.at(1)) == 0)){
                            stud.exams.push_back(e);
                            examExists = true;
                        }
                    }
                    if(!examExists){
                        cout << "Registration ignored: Exam with pnumber " << stoi(studentData.at(3)) << ", version " << studentData.at(2)
                        << " stg " << studentData.at(1) << " does not exist." << endl;
                    }
                }
            }
        }
    }
}

StudentParser::~StudentParser() = default;

vector<StudentParser::Student> &StudentParser::getStudents() {
    return students;
}

bool StudentParser::testTime(Time t, StudentParser::Student s){
    int count = 0;
    /*for(int i = 1; i <= 10; i++){
        if(i == t.day) count++;
        for(ExamParser::Exam& e : s.exams){
            if(i == e.examTime.day) count ++;
        }
        if(count > 2) return false;
        count = 0;
    }*/
    for(ExamParser::Exam& e : s.exams){
        if(!Time::diff(t, e.examTime, 240)){
            return false;
        }
    }
    return true;
}