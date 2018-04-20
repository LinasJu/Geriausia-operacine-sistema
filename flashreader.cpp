//
// Created by elsis on 18.4.15.
//

#include "flashreader.h"

FlashReader::FlashReader() {
    this->connected = false;
}

uint8_t FlashReader::read(uint32_t *destination, const uint8_t words) {
    if (!connected) {
        return 0;
    }

    // wordSize could be a global constant
    uint8_t wordSize = 4;

    auto *word = new uint8_t [wordSize];
    uint8_t readBytes = 0;

    for (uint8_t i = 0; i < words; i++) {
        device.read((char*)word, wordSize);
        if( device.eof() ) {
            break;
        }
        readBytes += 4;
        *(destination + i) = (word[0] << 24) + (word[1] << 16) + (word[2] << 8) + word[3];
    }

    delete[] word;

    return readBytes;
}

void FlashReader::connectToDevice(const std::string &devicePath) {
    if (!this->connected) {
        this->device.open(devicePath, std::ios::in | std::ios::binary);

        if (device.is_open()) {
            this->connected = true;
        }
        else {
            // error opening file
        }
    }
}

void FlashReader::disconnectFromDevice() {
    if (this->connected) {
        this->device.close();
        this->connected = false;
    }
}

bool FlashReader::isConnected() const {
    return connected;
}
