//
// Created by felix on 20/06/2020.
//

#include <iostream>
#include "../header/StudentParser.h"
using namespace std;

StudentParser::StudentParser(string fileName, std::unordered_map<std::string, ExamParser::Exam> providedExams) : CSVParser(fileName, separator::semicolon) {
    tableData tmp = getData();

    vector<string> fieldsOfStudy = getColumn(1);
    fieldsOfStudy = unique(fieldsOfStudy);

    for (int i = 0; i < fieldsOfStudy.size(); i++) {
        unordered_map<string, ExamParser::Exam> exam = {pair<string, ExamParser::Exam>("", ExamParser::Exam())};
        unordered_map<int, unordered_map<string, ExamParser::Exam>> tmp1 = {pair<int, unordered_map<string, ExamParser::Exam>>(0,exam)};
        pair<string, unordered_map<int, unordered_map<string, ExamParser::Exam>>> fieldOfStudyDefiner(fieldsOfStudy.at(i),tmp1);
        students.insert(fieldOfStudyDefiner);
    }

    for (int i = 1; i < tmp.size(); i++) {
        vector<string> studentData = getRow(i);

        // Setze Matrikelnummer, Studiengang
        int matrikelNumber = stoi(studentData.at(0));
        string fieldOfStudy = studentData.at(1);

        // Test ob Student bereits existiert
        bool studExists = students.at(fieldOfStudy).count(matrikelNumber);

        // Prüfung die angemeldet wird anlegen
        ExamParser::Exam exam = ExamParser::Exam{studentData.at(1), "", stoi(studentData.at(2)), stoi(studentData.at(3))};
        string key = exam.getKey();
        // Wenn die Prüfung nicht  existiert -> Fehler, Prüfung ignorieren
        if (!providedExams.count(key)) {
            cerr << "Registration ignored: Exam with pnumber " << stoi(studentData.at(3)) << ", version "
                 << studentData.at(2)
                 << " stg " << studentData.at(1) << " does not exist." << endl;
            continue;
        }

        // Wenn Student existiert, suche Student in Student-Vector und füge Prüfung hinzu
        if (studExists) {
                students.at(fieldOfStudy).at(matrikelNumber).insert({key, exam});
        } else {
            // Füge dem Student die Prüfung zu der die Anmeldung gehört hinzu
            students.at(fieldOfStudy).insert({matrikelNumber, {pair<string, ExamParser::Exam>(key, exam)}});
        }
    }
}

StudentParser::~StudentParser() = default;

unordered_map<string, unordered_map<int, unordered_map<string, ExamParser::Exam>>> StudentParser::getStudents() {
    return students;
}

bool StudentParser::testTime(Time t, string fieldOfStudy, int matrikelNumber){
    for(auto& element : students.at(fieldOfStudy).at(matrikelNumber)){
        if(!Time::diff(t, element.second.examTime, 240)){
            return false;
        }
    }
    return true;
}

vector<string> StudentParser::unique(const vector<string>& input) {
    std::vector<string> result;
    result.push_back(input.at(1));
    for (auto iter = input.begin()+2; iter != input.end();) {
        for (const auto& e : result){
            if (*iter != e){
                result.push_back(*iter);
                goto outer;
            }
        }
        outer: ++iter;
    }
    return result;
}