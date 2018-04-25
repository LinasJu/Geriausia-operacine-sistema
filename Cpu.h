//
// Created by karolis on 18.4.12.
//

#ifndef OS_CPU_H
#define OS_CPU_H

#include <cstdint>
#include <iostream>

class Cpu {
private:
    uint32_t PLR;
    uint16_t PC;
    uint16_t SP;
    uint16_t SM;
    uint8_t CX;
    uint8_t CH1;
    uint8_t CH2;
    uint8_t CH3;
    uint8_t IOI;
    uint8_t SF;
    uint8_t PID;
    uint8_t MODE;
    int8_t TI;
    uint8_t PI;
    uint8_t SI;

public:
    void TIpp();

    Cpu();

/*      SETTERS     */
    void setPLR(uint32_t);
    void setPLR1(uint8_t);
    void setPLR2(uint8_t);
    void setPLR3(uint8_t);
    void setPLR4(uint8_t);

    void setPC(uint16_t);
    void setPC1(uint8_t);
    void setPC2(uint8_t);

    void incPC();

    void setSP(uint16_t);
    void setSP1(uint8_t);
    void setSP2(uint8_t);

    void decSP();
    void incSP();

    void setSM(uint16_t);
    void setSM1(uint8_t);
    void setSM2(uint8_t);

    void setCH1(uint16_t);
    void setCH2(uint8_t);
    void setCH3(uint8_t);

    void setCX(uint8_t);

    void setIOI(uint8_t);

    void setSF(uint8_t);
    void setZF(bool i);
    void setCF(bool i);
    void setSiF(bool i);
    void setOF(bool i);

    void setPID(uint8_t);

    void setMODE(uint8_t);

    void setTI(int8_t);

    void setPI(uint8_t);

    void setSI(uint8_t);


/*      GETTERS     */
    uint32_t getPLR();
    uint8_t getPLR1();
    uint8_t getPLR2();
    uint8_t getPLR3();
    uint8_t getPLR4();

    uint16_t getPC();

    uint8_t getPC1();
    uint8_t getPC2();

    uint16_t getSP();
    uint8_t getSP1();
    uint8_t getSP1plus1();
    uint8_t getSP1minus1();
    uint8_t getSP2();

    uint16_t getSM();
    uint8_t getSM1();
    uint8_t getSM2();

    uint8_t getCX();

    uint16_t getCH1();
    uint8_t getCH2();
    uint8_t getCH3();

    uint8_t getIOI();

    uint8_t getSF();
    bool getZF();
    bool getCF();
    bool getSiF();
    bool getOF();

    void clearSF();

    uint8_t getPID();

    uint8_t getMODE();

    int8_t getTI();

    uint8_t getPI();

    uint8_t getSI();

};


#endif //OS_CPU_H
