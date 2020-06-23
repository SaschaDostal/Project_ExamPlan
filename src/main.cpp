#include <iostream>
#include "../header/CSVParser.h"
#include "../header/CSVWriter.h"
#include "../header/ExamParser.h"
#include "../header/StudentParser.h"
#include "../header/RoomParser.h"

#include <string>

using namespace std;

int main() {
    //example use of CSVParser:
    string fileName = "../InputData/Raumliste.csv";
    CSVParser csvParser(fileName, CSVParser::separator::comma);

    //example use of examParser:
    string examFile = "../InputData/Angebotene_Prüfungen_KL.csv";
    ExamParser examParser(examFile);

    //example use of examParser get Function:
    vector<ExamParser::Exam> exams = examParser.getExams();
    cout << exams.at(23).examName << endl;

    string studentFile = "../InputData/Anmeldungen_WS2019_KL.csv";
    StudentParser studentParser(studentFile, exams);

    for(ExamParser::Exam& e : examParser.getExams()){
        bool run = true;
        if(e.planned) run = false;
        int day = 1;
        int min = 0;
        while(run) {
            bool valid = true;
            for (StudentParser::Student& s : studentParser.getStudents()) {
                bool studentHasExamE = false;
                for (ExamParser::Exam& ex : s.exams){
                    if ((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))
                        || (e.examName == ex.examName)) {
                        studentHasExamE = true;
                    }
                }
                if (!studentParser.testTime(Time(day, min, e.examLength), s) && studentHasExamE) valid = false;
            }
            if(!valid){
                min += 15;
                if(min >= 600){
                    min -=600;
                    day++;
                }
            } else {
                e.examTime = Time(day, min, e.examLength);
                for(ExamParser::Exam& ex : examParser.getExams()){
                    if((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))
                        || (e.examName == ex.examName)){
                        ex.examTime = Time(day, min, e.examLength);
                        ex.planned = true;
                        cout << "Time scheduled: Day: " << ex.examTime.day << " Min: " << ex.examTime.min
                            << ", duration " << ex.examTime.duration << " Exam: " << ex.examNumber << " " << ex.examName  << endl;
                    }
                }
                for (StudentParser::Student& s : studentParser.getStudents()) {
                    for(ExamParser::Exam& ex : s.exams){
                        if((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))
                            || (e.examName == ex.examName)){
                            ex.examTime = Time(day, min, e.examLength);
                            ex.planned = true;
                        }
                    }
                }
                run = false;
            }
        }
    }


    /*example use of CSVParser get Function:
    vector<string> out;
    out = csvParser.getColumn(2);

    for (const auto& elem : out){
        cout << elem << " || ";
    }
     */
}
