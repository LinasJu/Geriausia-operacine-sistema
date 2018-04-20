//
// Created by elsis on 18.4.19.
//

#ifndef BABYOS_INPUTDEVICE_H
#define BABYOS_INPUTDEVICE_H

#include <cstdint>

#include "device.h"

class InputDevice: public Device {
public:
    virtual ~InputDevice() = default;
    virtual uint8_t read(uint32_t *destination, uint8_t words) = 0;
};


#endif //BABYOS_INPUTDEVICE_H
