//
// Created by 91dosa1bif on 22.06.20.
//

#ifndef ABSCHLUSSAUFGABE_TIME_H
#define ABSCHLUSSAUFGABE_TIME_H

#include <ostream>

class Time {
public:
    Time(int day, int min, int duration);
    Time();
    // Gibt true zurück, wenn Time a und b mindestens "min" minuten auseinander liegen
    static bool diff(Time a, Time b, int diff);
    // Start der Prüfung
    // Prüfungstage 1-10;
    int day;
    // Minute 0-599; Minute 0 entspricht 8 Uhr, Minute 600 entspricht 18 Uhr
    int min;
    // Dauer
    int duration;
    friend std::ostream& operator <<(std::ostream &out, const Time &time);
    friend bool operator ==(const Time &orig, const Time &other);
    virtual ~Time();
private:
};

#endif //ABSCHLUSSAUFGABE_TIME_H
