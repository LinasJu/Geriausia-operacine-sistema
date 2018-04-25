//
// Created by elsis on 18.4.15.
//

#include "flashreader.h"

FlashReader::FlashReader() {
    this->connected = false;
}

FlashReader::~FlashReader() {
    if (device.is_open()) {
        device.close();
    }
}

void FlashReader::read(char *destination, const uint16_t bytes) {
    if (!connected) {
        return;
    }

    device.read(destination, bytes);
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

uint16_t FlashReader::getFileSize() {
    if (connected) {
        device.seekg (0, std::ios::end);
        auto size = static_cast<uint16_t>(device.tellg());
        device.seekg (0, std::ios::beg);
        return size;
    }
    return 0;
}