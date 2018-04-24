//
// Created by elsis on 18.4.15.
//

#ifndef BABYOS_FLASHREADER_H
#define BABYOS_FLASHREADER_H

#include <cstdint>
#include <string>
#include <fstream>

#include "inputdevice.h"

/*
 * FlashReader is device driver for USB flash plug
 * FlashReader holds a state if USB is connected
 * FlashReader may hold a state if data can be read from device
 *
 * FlashReader communicates with CPU through a channel interface
 */
class FlashReader: public InputDevice {
public:
    FlashReader();
    ~FlashReader() override;
    void read(char *destination, uint16_t bytes) override;
    void connectToDevice(const std::string &devicePath);
    void disconnectFromDevice();
    uint16_t getFileSize();

    bool isConnected() const;

private:
    bool connected;
    std::ifstream device;
};


#endif //BABYOS_FLASHREADER_H
