//
// Created by 91dosa1bif on 22.06.20.
//

#include "../header/Time.h"

Time::Time(int day, int min) {
    this->day = day;
    this->min = min;
}

bool Time::diff(Time a, Time b, int min) {
    if(a.day != b.day) return true;
    if(a.min > b.min){
        if((a.min - b.min) < min) return false;
    } else {
        if((b.min - a.min) < min) return false;
    }
    return true;
}