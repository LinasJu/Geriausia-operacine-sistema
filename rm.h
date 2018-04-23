#ifndef RM_H
#define RM_H
#include "memory.h"
#include <cstdint>


class RM
{
public:
    RM();
private:
    uint32_t PLR;
    uint16_t PC;
    uint16_t SP;
    uint16_t SM;
    uint8_t CH1;
    uint8_t CH2;
    uint8_t CH3;
    uint8_t CH4;
    uint8_t IOI;
    uint8_t SF;
    uint8_t PID;
    uint8_t MODE;
    uint8_t TI;
    uint8_t PI;
    uint8_t SI;
    Memory mem;

};

#endif // RM_H
