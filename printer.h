//
// Created by elsis on 18.4.17.
//

#ifndef BABYOS_PRINTER_H
#define BABYOS_PRINTER_H

#include "outputdevice.h"

class Printer: public OutputDevice {
public:
    void print(const uint32_t *source, uint8_t words) override;
};


#endif //BABYOS_PRINTER_H
