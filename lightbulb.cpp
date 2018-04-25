#include "lightbulb.h"

Lightbulb::Lightbulb()
{
    this->state=0;
}
void Lightbulb::print(const uint32_t *source, uint8_t words){

}
void Lightbulb::print(uint32_t source){
    this->state=source;
}
int Lightbulb::getState(){
    return state;
}
