//
// Created by 91dosa1bif on 22.06.20.
//

#include "../header/Time.h"

Time::Time(int day, int min, int duration) {
    this->day = day;
    this->min = min;
    this->duration = duration;
}

Time::Time(){
    day = 0;
    min = 0;
    duration = 0;
}

bool Time::diff(Time a, Time b, int diff) {
    // Test unterschiedlicher Tag
    if(a.day != b.day) return true;
    // Prüfen welche Prüfung früher startet
    if(a.min < b.min){
        // Prüfen ob Start von a + Dauer + Differenz <= Start von b
        if(a.min + a.duration + diff <= b.min) return true;
    } else {
        if(b.min + b.duration + diff <= a.min) return true;
    }
    return false;
}

std::ostream& operator <<(std::ostream &out, const Time &time){
    char buf[5] = {0};
    sprintf(buf,"%02d:%02d", time.min/60 + 8, time.min%60);
    out << time.day << ";" << buf;
    return out;
}

bool operator ==(const Time &orig, const Time &other){
    return (orig.day == other.day) && (orig.min == other.min);
}

Time::~Time() = default;