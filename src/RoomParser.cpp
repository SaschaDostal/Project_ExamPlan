//
// Created by felix on 20/06/2020.
//

#include "../header/RoomParser.h"
#include "../header/CSVParser.h"
#include <algorithm>
using namespace std;

RoomParser::RoomParser(string fileName) : CSVParser(fileName, separator::semicolon){
    tableData tmp = getData();
    for(int i=1; i<tmp.size(); i++){
        Room room = {};
        vector<string> roomData = getRow(i);
        {
            room.description = roomData.at(0);
            room.location = roomData.at(1);
            room.seatCount = (stoi(roomData.at(2)))/4;
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
    std::sort(r.begin(), r.end(),[](RoomParser::Room a, RoomParser::Room b) { return (a.seatCount < b.seatCount);});
    vector<RoomParser::Room> biggestRooms;
    for(int i = 0; i < n; i++){
        r.at(i).examinerNr = i+1;
        biggestRooms.push_back(r.at(i));
    }
    return biggestRooms;
}