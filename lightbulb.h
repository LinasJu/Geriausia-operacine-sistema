#ifndef LIGHTBULB_H
#define LIGHTBULB_H
#include "outputdevice.h"

class Lightbulb : public OutputDevice
{
private:
    int state;
public:
    Lightbulb();
    void print(const uint32_t *source, uint8_t words) override;
    void print(uint32_t source);//sets state
    int getState();
};

#endif // LIGHTBULB_H
