#include <iostream>
#include <iomanip>
#include "../header/CSVParser.h"
#include "../header/CSVWriter.h"
#include "../header/ExamParser.h"
#include "../header/StudentParser.h"
#include "../header/RoomParser.h"

#include <string>

using namespace std;

int main() {
    //example use of roomParser:
    string roomFile = "../InputData/Raumliste.csv";
    RoomParser roomParser(roomFile);
    vector<RoomParser::Room> biggestNRooms = roomParser.getNBiggestRooms(20);

    //example use of examParser:
    string examFile = "../InputData/Angebotene_Prüfungen_KL.csv";
    ExamParser examParser(examFile);

    //example use of examParser get Function:
    vector<ExamParser::Exam> exams = examParser.getExams();
    vector<ExamParser::Exam> notPlannedExams;

    string studentFile = "../InputData/Anmeldungen_WS2019_KL.csv";
    StudentParser studentParser(studentFile, exams);

    // Schleife um allen Prüfungen einen Termin zuzuordnen
    for(ExamParser::Exam& e : examParser.getExams()){
        bool run = true;
        if(e.planned) run = false;  // Wenn die Klausur bereits geplant ist, keinen Termin suchen
        int day = 1;
        int min = 0;
        // Schleife für Terminsuche
        while(run) {
            bool valid = true;
            int numberOfStud = 0;
            // Schleife um Gültigkeit eines Termins am Tag "day" um Zeit "min" bei allen Studenten zu prüfen
            for (StudentParser::Student& s : studentParser.getStudents()) {
                bool studentHasExamE = false;
                for (ExamParser::Exam& ex : s.exams){
                    if ((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))
                        || (e.examName == ex.examName)) {
                        studentHasExamE = true;
                        numberOfStud++; // Anzahl der Studenten für Klausur erhöhen wenn Student Klausur schreibt
                    }
                }
                if (!studentParser.testTime(Time(day, min, e.examLength), s) && studentHasExamE) valid = false;
            }

            // Wenn "valid", dann getRoomsForNStudents()
            if(valid){
                vector<RoomParser::Room> examRooms = RoomParser::getRoomsForNStudents(numberOfStud, Time(day, min, e.examLength), biggestNRooms);
                // Wenn getRoomsForNStudents() kein Raum zurückgibt -> valid = false;
                if(examRooms.size() < 1){
                    valid = false;
                // Wenn getRoomsForNStudents() Räume zurück gibt, Prüfung in Liste room->exams und in String exam->room eintragen
                } else {
                    for(RoomParser::Room r1 : examRooms){
                        for(RoomParser::Room r2 : biggestNRooms){
                            if(r1.location.compare(r2.location) == 0) r2.exams.push_back(e);
                        }
                        e.rooms.append(" ");
                        e.rooms.append(r1.location);
                    }
                }
            }

            // Wenn der Termin nicht bei allen Studenten gültig ist, nächsten Termin wählen
            if(!valid){
                min += 15;
                if(min + e.examLength >= 600){
                    min -= (600 - e.examLength);
                    day++;
                }
                if(day > 10) day = 0;
                // Wenn alle 10 Tage durchlaufen wurden ohne dass ein Termin gefunden wurde, Exam der Liste notPlannedExams hinzufügen
                if(day == 1 && min == 0){
                    cout << "Error: No valid time found for Exam!" << endl;
                    notPlannedExams.push_back(e);
                    run = false;
                }
            // Wenn der Termin gültig ist, dann plane den Termin
            } else {
                e.examTime = Time(day, min, e.examLength);
                // Aktualisieren der Uhrzeit für alle Klausuren in der Klausurenliste mit gleicher Nummer, Version, Studiengang
                for(ExamParser::Exam& ex : examParser.getExams()){
                    if((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))){
                        ex.examTime = Time(day, min, e.examLength);
                        ex.planned = true;
                        ex.rooms = e.rooms;
                        cout << "Time scheduled: Day: " << ex.examTime.day << " Time: " << std::setfill('0')
                            << setw(2) << (int) ex.examTime.min/60 + 8 << ":" << setw(2) << ex.examTime.min % 60
                            << ", duration " << ex.examTime.duration << " Exam: " << ex.examNumber << " "
                            << ex.examName  << " Room(s):" << ex.rooms << endl;
                    }
                }
                // Aktualisieren der Zeit der Klausur für alle Studenten
                for (StudentParser::Student& s : studentParser.getStudents()) {
                    for(ExamParser::Exam& ex : s.exams){
                        if((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))){
                            ex.examTime = Time(day, min, e.examLength);
                            ex.planned = true;
                        }
                    }
                }
                run = false;
            }
        }
    }

    cout << "Not plannable exams: " << endl;
    for(ExamParser::Exam ex : notPlannedExams){
        cout << ex.examName << "Field of study: " << ex.fieldOfStudy << " Number: " << ex.examNumber << " Version: "
            << ex.examVersion << endl;
    }
}
