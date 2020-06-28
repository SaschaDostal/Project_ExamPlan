#include <iostream>
#include <iomanip>
#include "../header/CSVParser.h"
#include "../header/CSVWriter.h"
#include "../header/ExamParser.h"
#include "../header/StudentParser.h"
#include "../header/RoomParser.h"

#include <string>
#include <thread>
#include <mutex>

using namespace std;

vector<std::vector<ExamParser::Exam>> splitExams(const std::vector<ExamParser::Exam>& vec, size_t n);

int main() {
    //example use of roomParser:
    string roomFile = "../InputData/Raumliste.csv";
    RoomParser roomParser(roomFile);
    vector<RoomParser::Room> biggestNRooms = roomParser.getNBiggestRooms(20);

    //example use of examParser:
    string examFile = "../InputData/Angebotene_Prüfungen_KL.csv";
    ExamParser examParser(examFile);
    unordered_map<string, ExamParser::Exam> allExams = examParser.getExams();
    vector<ExamParser::Exam> notPlannedExams;

    string studentFile = "../InputData/Anmeldungen_WS2019_KL.csv";
    StudentParser studentParser(studentFile, allExams);

    unordered_map<string, unordered_map<int, unordered_map<string, ExamParser::Exam>>> students = studentParser.getStudents();

    // TODO Sortieren der Prüfungen von "allExams" nach examLength

    time_t before = time(nullptr);

    // Schleife um allen Prüfungen einen Termin zuzuordnen
    int examsWithoutStudents = 0;
    for(auto& e : allExams) {
        bool run = true;
        if (e.second.planned) run = false;  // Wenn die Klausur bereits geplant ist, keinen Termin suchen
        int day = 1;
        int min = 0;
        // Schleife für Terminsuche
        loop:
        while (run) {
            bool valid = true;
            // Schleife um Gültigkeit eines Termins am Tag "day" um Zeit "min" bei allen Studenten des Studiengangs zu prüfen
            vector<int> studentsParticipating;
            for (auto& students_ : students.at(e.second.fieldOfStudy)) {
                // Wenn Student die Prüfung schreibt
                if (students_.second.contains(e.first)) {
                    studentsParticipating.push_back(students_.first);
                    // Wenn der Student keine Zeit hat -> ungütiger Termin
                    if (!studentParser.testTime(Time(day, min, e.second.examLength), e.second.fieldOfStudy, students_.first)) valid = false;
                }
            }
            if(studentsParticipating.empty()) {
                examsWithoutStudents++;
                run = false;
                goto loop;
            }
            // Wenn "valid", dann getRoomsForNStudents()
            if (valid) {
                vector<RoomParser::Room> examRooms = RoomParser::getRoomsForNStudents(studentsParticipating.size(),
                                                                                      Time(day, min, e.second.examLength),
                                                                                      biggestNRooms);
                // Wenn getRoomsForNStudents() kein Raum zurückgibt -> valid = false;
                if (examRooms.empty()) {
                    goto elseBlock;
                // Wenn getRoomsForNStudents() Räume zurück gibt, dann plane Termin in Prüfung
                } else {
                    e.second.examTime = Time(day, min, e.second.examLength);
                    // Prüfung in Liste room->exams und in String exam->room eintragen
                    for (RoomParser::Room &r1 : examRooms) {
                        for (RoomParser::Room &r2 : biggestNRooms) {
                            if (r1.location == r2.location) r2.exams.push_back(e.second);
                        }
                        e.second.rooms.append(" ");
                        e.second.rooms.append(r1.location);
                    }
                }

                // Wenn der Termin gültig ist, dann plane den Termin
                cout << "Time scheduled: Day: " << e.second.examTime.day << " Time: " << std::setfill('0')
                     << setw(2) << (int) e.second.examTime.min / 60 + 8 << ":" << setw(2) << e.second.examTime.min % 60
                     << ", duration " << e.second.examTime.duration << " Exam: " << e.second.examNumber << " "
                     << e.second.examName << ", Room(s):" << e.second.rooms << endl;

                // Aktualisieren der Zeit der Klausur für alle Studenten die mitschreiben
                for (int matrikelNumber : studentsParticipating){
                    students.at(e.second.fieldOfStudy).at(matrikelNumber).at(e.second.getKey()).examTime = e.second.examTime;
                    students.at(e.second.fieldOfStudy).at(matrikelNumber).at(e.second.getKey()).planned = true;
                }
                run = false;
            }
            // Wenn der Termin nicht bei allen Studenten gültig ist, nächsten Termin wählen
            else {
                elseBlock:
                min += 15;
                if (min + e.second.examLength >= 600) {
                    min -= (600 - e.second.examLength);
                    day++;
                }
                if (day > 10) day = 1;
                // Wenn alle 10 Tage durchlaufen wurden ohne dass ein Termin gefunden wurde, Exam der Liste notPlannedExams hinzufügen
                if (day == 1 && min == 0) {
                    cerr << "Error: No valid time found for Exam!" << endl;
                    notPlannedExams.push_back(e.second);
                    run = false;
                }
            }
        }
    }

    cout << "Number of not plannable exams : " << notPlannedExams.size() << endl;
    if (!notPlannedExams.empty()){
        cout << "Not plannable exams: " << endl;
        for(const ExamParser::Exam& ex : notPlannedExams){
            cout << ex.examName << "Field of study: " << ex.fieldOfStudy << " Number: " << ex.examNumber << " Version: "
                 << ex.examVersion << endl;
        }
    }

    cout << "Number of exams without students: " << examsWithoutStudents << endl;
    
    // TODO Sortieren der Prüfungen von "allExams" nach examTime (wie unten nur jetzt eben mit der unordered_map)
    /*std::sort(allExams.begin(), allExams.end(),
              [](const ExamParser::Exam& a, const ExamParser::Exam& b)
                { return (a.examTime.day == b.examTime.day)? (a.examTime.min > b.examTime.min) : (a.examTime.day > b.examTime.day);});*/
    CSVWriter writer(allExams);
}

vector<std::vector<ExamParser::Exam>> splitExams(const std::vector<ExamParser::Exam>& vec, size_t n){
    std::vector<std::vector<ExamParser::Exam>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(n, vec.size()); ++i)
    {
        end += (remain > 0) ? (length + (remain-- != 0)) : length;

        outVec.emplace_back(vec.begin() + begin, vec.begin() + end);

        begin = end;
    }

    return outVec;
}
