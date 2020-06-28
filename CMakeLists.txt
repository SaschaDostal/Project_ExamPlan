cmake_minimum_required(VERSION 3.16)
project(Abschlussaufgabe)

set(CMAKE_CXX_STANDARD 17)

add_executable(Abschlussaufgabe src/main.cpp header/CSVParser.h header/ExamParser.h header/StudentParser.h header/RoomParser.h header/CSVWriter.h src/CSVParser.cpp src/CSVWriter.cpp src/ExamParser.cpp src/StudentParser.cpp src/RoomParser.cpp header/Time.h src/Time.cpp)