//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_ROOMPARSER_H
#define ABSCHLUSSAUFGABE_ROOMPARSER_H

#include "CSVParser.h"
#include "Time.h"
#include "ExamParser.h"

class RoomParser : CSVParser {
public:
    struct Room {
        std::string description;
        std::string location; // could also be a struct: e.g. struct location { int building, int floor, int room};
        int seatCount;
        int examinerNr;
        std::vector<ExamParser::Exam> exams;
    };
    RoomParser(std::string fileName);
    virtual ~RoomParser();
    std::vector<Room> getRooms();
    std::vector<Room> getNBiggestRooms(int n);
    static std::vector<Room> getRoomsForNStudents(int n, Time t, std::vector<RoomParser::Room>* r, ExamParser::Exam* lastExam);
private:
    std::vector<Room> rooms;
    int roomCount;
    bool testTime(Time t,  Room r);
};

#endif //ABSCHLUSSAUFGABE_ROOMPARSER_H
