//
// Created by karolis on 18.4.12.
//

#include "Cpu.h"

Cpu::Cpu(){
    this->PLR = 0;
    this->PC = 0;
    this->SP = 0;
    this->CH1 = 0;
    this->CH2 = 0;
    this->CH3 = 0;
    this->CH4 = 0;
    this->IOI = 0;
    this->SF = 0;
    this->PID = 0;
    this->MODE = 0;
    this->TI = 10;
    this->PI = 0;
    this->SI = 0;
    this->CX = 0;
}


/*      SETTERS     */
void Cpu::setPLR(uint32_t PLR){
    this->PLR = PLR;
}

void Cpu::setPLR1(uint8_t byte){
    this->PLR = this->PLR & 0xFFFFFF00 | byte;
}

void Cpu::setPLR2(uint8_t byte){
    this->PLR = this->PLR & 0xFFFF00FF | (uint16_t(byte) << 8);
}

void Cpu::setPLR3(uint8_t byte){
    this->PLR = this->PLR & 0xFF00FFFF | (uint32_t(byte) << 16);
}

void Cpu::setPLR4(uint8_t byte){
    this->PLR = this->PLR & 0x00FFFFFF | (uint16_t(byte) << 24);
}

void Cpu::setPC(uint16_t PC){
    this->PC = PC;
}

void Cpu::setPC1(uint8_t byte){
    if(byte >= 0x6F){
        PI = 1;
    }
    this->PC = this->PC & 0xFFFFFF00 | byte;
}

void Cpu::setPC2(uint8_t byte){
    this->PC = this->PC & 0xFFFF00FF | (uint16_t(byte) << 8);
}

void Cpu::incPC()
{
    PC++;
    if(getPC1() > 0x6F){
        PI = 1;
    }
}

void Cpu::setSP(uint16_t SP){
    this->SP = SP;
}

void Cpu::setSP1(uint8_t byte){
    this->SP = this->SP & 0xFFFFFF00 | byte;
}

void Cpu::setSP2(uint8_t byte){
    this->SP = this->SP & 0xFFFF00FF | (uint16_t(byte) << 8);
}

void Cpu::decSP()
{
    SP--;
    if(SP < 0){
        PI = 1;
    }
}

void Cpu::incSP()
{
    SP++;
    if(getSP1() == 0){
        PI = 4;
    }
}

void Cpu::setCH1(uint8_t CH1){
    this->CH1 = CH1;
}

void Cpu::setCH2(uint8_t CH2){
    this->CH2 = CH2;
}

void Cpu::setCH3(uint8_t CH3){
    this->CH3 = CH3;
}

void Cpu::setCH4(uint8_t CH4){
    this->CH4 = CH4;
}

void Cpu::setIOI(uint8_t IOI){
    this->IOI = IOI;
}

void Cpu::setSF(uint8_t SF){
    this->SF = SF;
}

void Cpu::setZF(bool flag){
    if(flag) {
        this->SF |= 1u;
    }else{
        this->SF &= ~1u;
    }
}

void Cpu::setCX(uint8_t cx){
    this->CX=cx;
}

void Cpu::setCF(bool flag){
    if(flag) {
        this->SF |= (1u << 1);
    }else{
        this->SF &= ~(1u << 1);
    }
}

void Cpu::setSiF(bool flag){
    if(flag) {
        this->SF |= (1u << 2);
    }else{
        this->SF &= ~(1u << 2);
    }
}

void Cpu::setOF(bool flag){
    if(flag) {
        this->SF |= (1u << 3);
    }else{
        this->SF &= ~(1u << 3);
    }
}

void Cpu::setPID(uint8_t PID){
    this->PID = PID;
}

void Cpu::setMODE(uint8_t MODE){
    this->MODE = MODE;
}

void Cpu::setTI(int8_t TI){
    this->TI = TI;
}

void Cpu::setPI(uint8_t PI){
    this->PI = PI;
}

/**
 * SI = 1 PRINTS
 * SI = 2 PRINTN
 * SI = 3 READ
 * SI = 4 HALT
*/
void Cpu::setSI(uint8_t SI){
    this->SI = SI;
}


/*      GETTERS     */
uint32_t Cpu::getPLR(){
    return this->PLR;
}

uint8_t Cpu::getPLR1(){
    return (this->PLR & 0xFF);
}

uint8_t Cpu::getPLR2(){
    return uint8_t((this->PLR >> 8) & 0xFF);
}

uint8_t Cpu::getPLR3(){
    return uint8_t((this->PLR >> 16) & 0xFF);
}

uint8_t Cpu::getPLR4(){
    return ((this->PLR >> 24) & 0xFF);
}

uint16_t Cpu::getPC(){
    return this->PC;
}

uint8_t Cpu::getPC2(){
    return uint8_t((this->PC >> 8) & 0xFF);
}

uint8_t Cpu::getPC1(){
    return uint8_t(this->PC & 0x00FF);
}

uint16_t Cpu::getSP(){
    return this->SP;
}

uint8_t Cpu::getSP1(){
    return uint8_t(this->SP & 0x00FF);
}

uint8_t Cpu::getSP1plus1()
{
    if(getSP1() + 1 > 255){
        PI = 1;
    }
    return uint8_t(getSP1() + 1 & 0x00FF);
}

uint8_t Cpu::getSP1minus1()
{
    if(getSP1() - 1 < 0){
        PI = 1;
    }
    return uint8_t(getSP1() - 1 & 0x00FF);
}

uint8_t Cpu::getSP2(){
    return uint8_t((this->SP >> 8) & 0xFF);
}
uint8_t Cpu::getCX(){
    return this->CX;
}
uint16_t Cpu::getCH1(){
    return this->CH1;
}

uint8_t Cpu::getCH2(){
    return this->CH2;
}

uint8_t Cpu::getCH3(){
    return this->CH3;
}

uint8_t Cpu::getCH4(){
    return this->CH4;
}

uint8_t Cpu::getIOI(){
    return this->IOI;
}

uint8_t Cpu::getSF(){
    return this->SF;
}

bool Cpu::getZF(){
    return (this->SF & 1) == 1;
}

bool Cpu::getCF(){
    return (this->SF & 2) == 2;
}

bool Cpu::getSiF(){
    return (this->SF & 4) == 4;
}

bool Cpu::getOF(){
    return (this->SF & 8) == 8;
}


uint8_t Cpu::getPID(){
    return this->PID;
}

uint8_t Cpu::getMODE(){
    return this->MODE;
}

int8_t Cpu::getTI(){
    return this->TI;
}

uint8_t Cpu::getPI(){
    return this->PI;
}

uint8_t Cpu::getSI(){
    return this->SI;
}


void Cpu::clearSF()
{
    this->SF = 0;
}
void Cpu::decTI(int a){
    this->TI=this->TI-a;
}
