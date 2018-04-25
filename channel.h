//
// Created by elsis on 18.4.15.
//

#ifndef BABYOS_CHANNEL_H
#define BABYOS_CHANNEL_H

#include <cstdint>
#include <utility>

#include "device.h"

class Channel: public Device {
public:
    enum class DeviceType {
        READ_ONLY,
        WRITE_ONLY,
        READ_AND_WRITE
    };

    enum class ChannelType {
        INPUT_TO_MEMORY,
        MEMORY_TO_OUTPUT,
        MEMORY_TO_MEMORY,
        NOT_SET
    };

    Channel(uint8_t *channel_reg, uint8_t *ioi_reg, uint8_t ioi_increment);
    ~Channel();

    void addDevice(Device *device, DeviceType type);

    void getInput(uint32_t *destination, uint8_t words);
    void sendOutput(uint32_t *source, uint8_t words);
    
    //void exchangeMemory(uint32_t *source, uint32_t *destination, uint8_t words, bool ioflag);

private:
    void setChannelType();

    uint8_t * const channel_reg;
    uint8_t * const ioi_reg;
    uint8_t const ioi_increment;

    ChannelType channelType = ChannelType::NOT_SET;

    std::pair<Device*, DeviceType> *firstDevice = nullptr;
    std::pair<Device*, DeviceType> *secondDevice = nullptr;
};


#endif //BABYOS_CHANNEL_H
