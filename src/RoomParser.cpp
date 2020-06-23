//
// Created by felix on 20/06/2020.
//

#include "../header/RoomParser.h"
#include "../header/CSVParser.h"
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