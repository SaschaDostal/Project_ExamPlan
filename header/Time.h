//
// Created by 91dosa1bif on 22.06.20.
//

#ifndef ABSCHLUSSAUFGABE_TIME_H
#define ABSCHLUSSAUFGABE_TIME_H

class Time {
public:
    Time(int day, int min);
private:
    // Prüfungstage 1-10; day = 0, wenn Zeitdifferenz, z.B. Prüfungsdauer
    int day;
    // Minute 0-599; Minute 0 entspricht 8 Uhr, Minute 600 entspricht 18 Uhr
    int min;
    // Gibt true zurück, wenn Time a und b mindestens "min" minuten auseinander liegen
    static bool diff(Time a, Time b, int min);
};

#endif //ABSCHLUSSAUFGABE_TIME_H
