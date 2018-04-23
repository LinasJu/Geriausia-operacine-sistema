//
// Created by elsis on 18.4.15.
//

#include "channel.h"
#include "flashreader.h"
#include "printer.h"

Channel::Channel(uint8_t *channel_reg, uint8_t *ioi_reg, uint8_t ioi_increment)
        : channel_reg(channel_reg), ioi_reg(ioi_reg), ioi_increment(ioi_increment) {}

Channel::~Channel() {
    delete firstDevice;
    delete secondDevice;
}

void Channel::addDevice(Device* device, DeviceType type) {
    if (firstDevice == nullptr) {
        firstDevice = new std::pair<Device*, DeviceType>(device, type);
    }
    else if (secondDevice == nullptr) {
        secondDevice = new std::pair<Device*, DeviceType>(device, type);
    }
    else return;

    if (firstDevice != nullptr && secondDevice != nullptr)
        setChannelType();
}

void Channel::setChannelType() {
    if (firstDevice->second == DeviceType::READ_AND_WRITE) {
        switch (secondDevice->second) {
            case DeviceType::READ_ONLY :
                channelType = ChannelType::INPUT_TO_MEMORY;
            case DeviceType::WRITE_ONLY :
                channelType = ChannelType::MEMORY_TO_OUTPUT;
            case DeviceType::READ_AND_WRITE :
                channelType = ChannelType::MEMORY_TO_MEMORY;
        }
    }
    else if (secondDevice->second == DeviceType::READ_AND_WRITE) {
        switch (firstDevice->second) {
            case DeviceType::READ_ONLY :
                channelType = ChannelType::INPUT_TO_MEMORY;
            case DeviceType::WRITE_ONLY :
                channelType = ChannelType::MEMORY_TO_OUTPUT;
            case DeviceType::READ_AND_WRITE :
                channelType = ChannelType::MEMORY_TO_MEMORY;
        }
    }
    else {
        channelType = ChannelType::NOT_SET;
    }
}

void Channel::getInput(uint32_t *destination, uint8_t words) {
    *channel_reg = 1;

    if (channelType == ChannelType::INPUT_TO_MEMORY) {
        FlashReader *reader;
        if (firstDevice->second == DeviceType::READ_ONLY) {
            reader = (FlashReader*)firstDevice->first;
        }
        else {
            reader = (FlashReader*)secondDevice->first;
        }

        reader->read(destination, words);
    }

    *channel_reg = 0;
    *ioi_reg += ioi_increment;
}

void Channel::sendOutput(uint32_t *source, uint8_t words) {
    *channel_reg = 1;

    if (channelType == ChannelType::MEMORY_TO_OUTPUT) {
        Printer *printer;
        if (firstDevice->second == DeviceType::WRITE_ONLY) {
            printer = (Printer*)firstDevice->first;
        }
        else {
            printer = (Printer*)secondDevice->first;
        }

        printer->print(source, words);
    }

    *channel_reg = 0;
    *ioi_reg += ioi_increment;
}
/*
void Channel::exchangeMemory(uint32_t *source, uint32_t *destination, uint8_t words, bool ioflag) {
    *channel_reg = 1;

    if (channelType == ChannelType::MEMORY_TO_MEMORY) {
        Memory *sender;

        if (ioflag) {
            sender = (Memory*)firstDevice->first;
        }
        else {
            sender = (Memory*)secondDevice->first;
        }
    }

    *channel_reg = 0;
    *ioi_reg += ioi_increment;
}
*/