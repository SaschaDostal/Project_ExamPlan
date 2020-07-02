//
// Created by felix on 20/06/2020.
//

#include "../header/RoomParser.h"
#include "../header/CSVParser.h"
#include <iostream>
#include <algorithm>
using namespace std;

RoomParser::RoomParser(string fileName) : CSVParser(fileName, separator::comma){
    tableData tmp = getData();
    for(int i=0; i<tmp.size(); i++){
        Room room = {};
        vector<string> roomData = getRow(i);
        {
            room.description = roomData.at(0).substr(1, roomData.at(0).size() -2);
            room.location = roomData.at(1).substr(1, roomData.at(1).size() -2);
            room.seatCount = stoi(roomData.at(2).substr(1, roomData.at(2).size() -2)) / 4;
        }
        rooms.push_back(room);
    }
}

RoomParser::~RoomParser() = default;

vector<RoomParser::Room> RoomParser::getRooms() {
    return rooms;
}

vector<RoomParser::Room> RoomParser::getNBiggestRooms(int n){
    vector<RoomParser::Room> r = rooms;
    std::sort(r.begin(), r.end(),[](RoomParser::Room a, RoomParser::Room b) { return (a.seatCount > b.seatCount);});
    vector<RoomParser::Room> biggestRooms;
    if(n > r.size()){
        cerr << "Error: Asked for too many rooms" << endl;
        return biggestRooms;
    }
    for(int i = 0; i < n; i++){
        r.at(i).examinerNr = i+1;
        biggestRooms.push_back(r.at(i));
    }
    return biggestRooms;
}

vector<pair<int, RoomParser::Room>> RoomParser::getRoomsForNStudents(int n, const Time& t, std::vector<RoomParser::Room>* r, ExamParser::Exam *lastExam) {
    vector<pair<int, RoomParser::Room>> roomsForNStuds;
    int spaceReseved = 0;
    // Liste der Räume durchlaufen
    for(RoomParser::Room& room : *r){
        bool free = true;
        ExamParser::Exam *examWithFreeSpace = nullptr;
        vector<ExamParser::Exam> *examsptr = &room.exams;
        // Testen ob Zeit t für alle Prüfungen von raum "room" frei ist
        for(ExamParser::Exam& exam : *examsptr){
            if(!Time::diff(t, exam.examTime, 60)){
                free = false;
                if((exam.freeSpace > 0) && exam.examTime == t && exam.examLength == t.duration){
                    examWithFreeSpace = &exam;
                    break;
                }
            }
        }
        if((examWithFreeSpace != nullptr && examWithFreeSpace->freeSpace >= n) /*&& (examWithFreeSpace != lastExam)*/){
            roomsForNStuds.push_back({examWithFreeSpace->freeSpace, room});
            examWithFreeSpace->freeSpace = 0;
            lastExam->freeSpace = 0;
            return roomsForNStuds;
        }
        if(free){       // Wenn Raum frei ist, Raum zur Liste roomsForNStuds hinzufügen
            spaceReseved += room.seatCount;
            roomsForNStuds.push_back({room.seatCount, room});
        }
        if(spaceReseved >= n){      // Wenn genug Platz reserviert wurde, Liste mit Räumen zurückgeben
            lastExam->freeSpace = spaceReseved - n;
            return roomsForNStuds;
        }
    }
    //Wenn nicht genug Platz reserviert werden konnte, leere Liste zurückgeben
    vector<pair<int, RoomParser::Room>> empty;
    return empty;
}