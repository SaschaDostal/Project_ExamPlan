//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_ROOMPARSER_H
#define ABSCHLUSSAUFGABE_ROOMPARSER_H

#include "CSVParser.h"

class RoomParser : CSVParser {
public:
    struct room {
        char *description;
        char *location; // could also be a struct: e.g. struct location { int building, int floor, int room};
        char *seatCount;
    };
    RoomParser();
    virtual ~RoomParser();
    void getRooms(room *rooms, int &roomCount);
private:
    room *rooms;
    int roomCount;
};

#endif //ABSCHLUSSAUFGABE_ROOMPARSER_H
