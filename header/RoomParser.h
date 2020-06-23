//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_ROOMPARSER_H
#define ABSCHLUSSAUFGABE_ROOMPARSER_H

#include "CSVParser.h"

class RoomParser : CSVParser {
public:
    struct Room {
        std::string description;
        std::string location; // could also be a struct: e.g. struct location { int building, int floor, int room};
        int seatCount;
    };
    RoomParser(std::string fileName);
    virtual ~RoomParser();
    std::vector<Room> getRooms();
private:
    std::vector<Room> rooms;
    int roomCount;
};

#endif //ABSCHLUSSAUFGABE_ROOMPARSER_H
