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

void planADay(int day, StudentVector& students, vector<RoomParser::Room>& biggestNRooms, vector<ExamParser::Exam>& allExams, ExamVector& notPlannedExams);
vector<std::vector<ExamParser::Exam>> splitExams(const std::vector<ExamParser::Exam>& vec, size_t n);

int main() {
    //example use of roomParser:
    string roomFile = "../InputData/Raumliste.csv";
    RoomParser roomParser(roomFile);
    vector<RoomParser::Room> biggestNRooms = roomParser.getNBiggestRooms(20);

    //example use of examParser:
    string examFile = "../InputData/Angebotene_Prüfungen_KL.csv";
    ExamParser examParser(examFile);

    //example use of examParser get Function:
    vector<ExamParser::Exam> allExams = examParser.getExams();

    string studentFile = "../InputData/Anmeldungen_WS2019_KL.csv";
    StudentParser studentParser(studentFile, allExams);

    // TODO Sortieren der Prüfungen von "allExams" nach examLength

    time_t before = time(nullptr);

    // Schleife um allen Prüfungen einen Termin zuzuordnen
    int numOfDays = 10;
    vector<vector<ExamParser::Exam>> examSplit = splitExams(allExams, numOfDays);
    ExamVector notPlannedExams;
    StudentVector students = studentParser.getStudents();
    // vector<ExamParser::Exam> notPlannedExamReturn;

    thread threads[numOfDays];
    for (int i = 0; i < numOfDays; i++) {
        threads[i] = thread(planADay, i+1, ref(students), ref(biggestNRooms), ref(examSplit.at(i)), ref(notPlannedExams));
    }
    for (thread& t : threads){
        t.join();
    }
    for (int a=1; a<numOfDays; a++) {
        for (int i = 0; i < numOfDays; i++) {
            int x = (i+a+1 < numOfDays)? i+a+1 : (i+a+1)-numOfDays;
            threads[i] = thread(planADay, x, ref(students), ref(biggestNRooms), ref(examSplit.at(i)), ref(notPlannedExams));
        }
        for (thread& t : threads){
            t.join();
        }
    }

    cerr << "All days combined took: " << difftime(time(nullptr), before) << " seconds" << endl;

    cout << "Not plannable exams: " << endl;
    for(const ExamParser::Exam& ex : notPlannedExams){
        cout << ex.examName << "Field of study: " << ex.fieldOfStudy << " Number: " << ex.examNumber << " Version: "
            << ex.examVersion << endl;
    }
    cout << "Number of not plannable exams : " << notPlannedExams.size() << endl;

    std::sort(allExams.begin(), allExams.end(),
              [](const ExamParser::Exam& a, const ExamParser::Exam& b)
                { return (a.examTime.day == b.examTime.day)? (a.examTime.min > b.examTime.min) : (a.examTime.day > b.examTime.day);});
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

        outVec.push_back(std::vector<ExamParser::Exam>(vec.begin() + begin, vec.begin() + end));

        begin = end;
    }

    return outVec;
}

void planADay(int day, StudentVector& students, vector<RoomParser::Room>& biggestNRooms, ExamVector& myExams, ExamVector& notPlannedExams){

    vector<ExamParser::Exam> myNotPlannedExams;
    unique_lock<mutex> notPlannedExamsLock(notPlannedExams.examMutex, defer_lock);
    unique_lock<mutex> studentsLock(students.studentMutex, defer_lock);
    unique_lock<mutex> examsLock(myExams.examMutex, defer_lock);

    time_t before = time(nullptr);

    // Schleife um allen Prüfungen einen Termin zuzuordnen
    examsLock.lock();
    for(ExamParser::Exam& e : myExams){
        bool run = true;
        if(e.planned) run = false;  // Wenn die Klausur bereits geplant ist, keinen Termin suchen
        int min = 0;
        // Schleife für Terminsuche
        while(run) {
            bool valid = true;
            int numberOfStud = 0;
            // Schleife um Gültigkeit eines Termins am Tag "day" um Zeit "min" bei allen Studenten zu prüfen
            studentsLock.lock();
            for (StudentParser::Student& s : students) {
                bool studentHasExamE = false;
                for (ExamParser::Exam& ex : s.exams){
                    if ((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))
                        || (e.examName == ex.examName)) {
                        studentHasExamE = true;
                        numberOfStud++; // Anzahl der Studenten für Klausur erhöhen wenn Student Klausur schreibt
                    }
                }
                if (!StudentParser::testTime(Time(day, min, e.examLength), s) && studentHasExamE) valid = false;
            }
            studentsLock.unlock();

            // Wenn "valid", dann getRoomsForNStudents()
            if(valid){
                vector<RoomParser::Room> examRooms = RoomParser::getRoomsForNStudents(numberOfStud, Time(day, min, e.examLength), biggestNRooms);
                // Wenn getRoomsForNStudents() kein Raum zurückgibt -> valid = false;
                if(examRooms.empty()){
                    valid = false;
                    // Wenn getRoomsForNStudents() Räume zurück gibt, dann plane Termin in Prüfung
                    // Prüfung in Liste room->exams und in String exam->room eintragen
                } else {
                    e.examTime = Time(day, min, e.examLength);
                    for(RoomParser::Room& r1 : examRooms){
                        for(RoomParser::Room& r2 : biggestNRooms){
                            if(r1.location == r2.location) r2.exams.push_back(e);
                        }
                        e.rooms.append(" ");
                        e.rooms.append(r1.location);
                    }
                }
            }

            // Wenn der Termin nicht bei allen Studenten gültig ist, nächsten Termin wählen
            if(!valid){
                min += 15;
                // Wenn alle 10 Stunden durchlaufen wurden ohne dass ein Termin gefunden wurde, Exam der Liste notPlannedExams hinzufügen
                if(min + e.examLength >= 600){
                    cout << "Error: No valid time found for Exam!" << endl;
                    myNotPlannedExams.push_back(e);
                    run = false;
                }
                // Wenn der Termin gültig ist, dann plane den Termin
            } else {
                cout << "Time scheduled: Day: " << e.examTime.day << " Time: " << std::setfill('0')
                     << setw(2) << (int) e.examTime.min/60 + 8 << ":" << setw(2) << e.examTime.min % 60
                     << ", duration " << e.examTime.duration << " Exam: " << e.examNumber << " "
                     << e.examName  << ", Room(s):" << e.rooms << endl;

                // Aktualisieren der Zeit der Klausur für alle Studenten
                studentsLock.lock();
                for (StudentParser::Student& s : students) {
                    for(ExamParser::Exam& ex : s.exams){
                        if((e.examNumber == ex.examNumber && e.examVersion == ex.examVersion && (e.fieldOfStudy.compare(ex.fieldOfStudy) == 0))){
                            ex.examTime = Time(day, min, e.examLength);
                            ex.planned = true;
                        }
                    }

                }
                studentsLock.unlock();
                run = false;
            }
        }
    }
    examsLock.unlock();

    cerr << "Day " << day << " took: " << difftime(time(nullptr), before) << " seconds" << endl;

    cout << "Not plannable exams: " << endl;
    notPlannedExamsLock.lock();
    for(const ExamParser::Exam& ex : myNotPlannedExams){
        cout << ex.examName << "Field of study: " << ex.fieldOfStudy << " Number: " << ex.examNumber << " Version: "
             << ex.examVersion << endl;
        notPlannedExams.push_back(ex);
    }
    notPlannedExamsLock.unlock();
    cout << "Number of not plannable exams : " << notPlannedExams.size() << endl;
}