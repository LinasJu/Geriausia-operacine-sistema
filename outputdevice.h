//
// Created by elsis on 18.4.19.
//

#ifndef BABYOS_OUTPUTDEVICE_H
#define BABYOS_OUTPUTDEVICE_H

#include <cstdint>

#include "device.h"

class OutputDevice: public Device {
public:
    virtual ~OutputDevice() = default;
    virtual void print(const uint32_t *source, uint8_t words) = 0;

};


#endif //BABYOS_OUTPUTDEVICE_H
