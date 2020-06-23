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
        int day = 1;
        int min = 0;
        while(run) {
            bool valid = true;
            for (StudentParser::Student& s : studentParser.getStudents()) {
                if (!studentParser.testTime(Time(day, min, e.examLength), s)) valid = false;
            }
            if(!valid){
                min += 15;
                if(min >= 600){
                    min -=600;
                    day++;
                }
            } else {
                e.examTime = Time(day, min, e.examLength);
                for (StudentParser::Student& s : studentParser.getStudents()) {
                    for(ExamParser::Exam& ex : s.exams){
                        if(e.examNumber == ex.examNumber) ex.examTime = Time(day, min, e.examLength);
                    }
                }
                cout << "Time scheduled: exam " << e.examNumber << ", time Day " << e.examTime.day
                    << " Min " << e.examTime.min << ", duration " << e.examTime.duration << endl;
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
