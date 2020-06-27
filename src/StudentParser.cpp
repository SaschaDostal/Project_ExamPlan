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
        // Test ob Student bereits existiert
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
            // Füge dem Student die Prüfung zu der die Anmeldung gehört
            for(ExamParser::Exam& e : allExams){
                if(stoi(studentData.at(3)) == e.examNumber  && stoi(studentData.at(2)) == e.examVersion
                    && (e.fieldOfStudy.compare(studentData.at(1)) == 0)){
                    student.exams.push_back(e);
                    examExists = true;
                }
            }
            // Wenn die Klausur in der Anmeldung existiert, dann füge Student zur Liste der Studenten hinzu
            if(examExists){
                students.push_back(student);
            // Wenn dazugehörige Klausur nicht existiert gebe aus, dass Anmeldung ignoriert wurde
            } else {
                cout << "Registration ignored: Exam with pnumber " << stoi(studentData.at(3)) << ", version " << studentData.at(2)
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
                            student.exams.push_back(e);
                            examExists = true;
                        }
                    }
                    // Wenn dazugehörige Klausur nicht existiert gebe aus, dass Anmeldung ignoriert wurde
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
    for(const auto& e : s.exams){
        if(!Time::diff(t, e.examTime, 240)){
            return false;
        }
    }
    return true;
}