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
    virtual void read(char *destination, uint16_t bytes) = 0;
};


#endif //BABYOS_INPUTDEVICE_H
