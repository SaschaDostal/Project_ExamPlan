//
// Created by felix on 20/06/2020.
//

#ifndef ABSCHLUSSAUFGABE_CSVWRITER_H
#define ABSCHLUSSAUFGABE_CSVWRITER_H

#include <cstdio>

class CSVWriter {
public:
    enum class separator{ comma, semicolon};
    CSVWriter();
    CSVWriter(separator separator);
    virtual ~CSVWriter();

private:
    FILE *file;
};

#endif //ABSCHLUSSAUFGABE_CSVWRITER_H
