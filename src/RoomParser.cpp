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

vector<RoomParser::Room> RoomParser::getRoomsForNStudents(int n, Time t, std::vector<RoomParser::Room>& r) {
    vector<RoomParser::Room> roomsForNStuds;
    int spaceReseved = 0;
    // Liste der Räume durchlaufen
    for(RoomParser::Room& room : r){
        bool free = true;
        // Testen ob Zeit t für alle Prüfungen von raum "room" frei ist
        for(ExamParser::Exam& exam : room.exams){
            if(!Time::diff(t, exam.examTime, 60)){
                free = false;
            }
        }
        if(free){       // Wenn Raum frei ist, Raum zur Liste roomsForNStuds hinzufügen
            spaceReseved += room.seatCount;
            roomsForNStuds.push_back(room);
        }
        if(spaceReseved >= n){      // Wenn genug Platz reserviert wurde, Liste mit Räumen zurückgeben
            return roomsForNStuds;
        }
    }
    //Wenn nicht genug Platz reserviert werden konnte, leere Liste zurückgeben
    vector<RoomParser::Room> empty;
    return empty;
}