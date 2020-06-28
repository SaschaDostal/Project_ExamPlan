#include <iostream>
#include <iomanip>
#include "../header/CSVParser.h"
#include "../header/CSVWriter.h"
#include "../header/ExamParser.h"
#include "../header/StudentParser.h"
#include "../header/RoomParser.h"

#include <string>
#include <algorithm>

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
    /*int rows = allExams.size();
    bool isSorted = false ;
    while (true) {
        int u = 0;
        if(!isSorted) {
            isSorted = true;
            for (auto j = next(allExams.begin(),1) ; j != prev(allExams.end(),2); ++j) {

                int laenge = (j)->second.examLength;
                int laenge2 = ((next(j,1)))->second.examLength;

                if (laenge2 > laenge) {
                    vector<ExamParser::Exam> temp;
                    temp.push_back(((next(j,1)))->second);
                    ((next(j,1)))->second = (j)->second;
                    (j)->second = temp[0];
                    isSorted = false;
                }
            }

        } else { break;}
    }*/

    time_t before = time(nullptr);

    // Schleife um allen Prüfungen einen Termin zuzuordnen
    int examsWithoutStudents = 0;
    for(auto& e : allExams) {
        bool run = true;
        if(e.second.examLength == 0) run = false;
        if (e.second.planned) run = false;  // Wenn die Klausur bereits geplant ist, keinen Termin suchen
        int day = 1;
        int min = 0;
        // Schleife für Terminsuche
        while (run) {
            bool valid = true;
            // Schleife um Gültigkeit eines Termins am Tag "day" um Zeit "min" bei allen Studenten des Studiengangs zu prüfen
            vector<int> studentsParticipating;
            for (auto& students_ : students.at(e.second.fieldOfStudy)) {
                // Wenn Student die Prüfung schreibt
                if (students_.second.count(e.first)) {
                    studentsParticipating.push_back(students_.first);
                    // Wenn der Student keine Zeit hat -> ungütiger Termin
                    if (!StudentParser::testTime(Time(day, min, e.second.examLength), students_))
                        valid = false;
                }
            }
            if(studentsParticipating.empty()) {
                examsWithoutStudents++;
                run = false;
                break;
            }
            // Wenn "valid", dann getRoomsForNStudents()
            if (valid) {
                vector<RoomParser::Room> examRooms = RoomParser::getRoomsForNStudents(studentsParticipating.size(),
                                                                                      Time(day, min, e.second.examLength),
                                                                                      &biggestNRooms, &e.second);
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
                        e.second.roomCapacity += r1.seatCount;
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
                    e.second.mtknrs.append(to_string(matrikelNumber)+ ";");
                }
                e.second.planned = true;
                e.second.numStudents = studentsParticipating.size();
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
    vector<pair<string, ExamParser::Exam>> elems(allExams.begin(), allExams.end());

    sort(elems.begin(), elems.end(),
              [](const pair<string, ExamParser::Exam>& a, const pair<string,ExamParser::Exam>& b)
                { return (a.second.examTime.day == b.second.examTime.day)? (a.second.examTime.min < b.second.examTime.min) : (a.second.examTime.day < b.second.examTime.day);});

    CSVWriter writer(elems);

    printf("%f", difftime(before, time(nullptr)));
}
