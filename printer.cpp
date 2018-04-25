//
// Created by elsis on 18.4.17.
//
#include "printer.h"

void Printer::print(const uint32_t *source, uint8_t words) {
    for (uint8_t i = 0; i < words; i++) {
        std::cout << *(source + i) << std::endl;
    }
}
void Printer::print(const uint32_t *source, uint8_t words,MainWindow *w) {
    for (uint8_t i = 0; i < words; i++) {
        w->appendOutput(std::to_string(*(source+i)));
    }
}
void Printer::print(const uint32_t source,MainWindow *w) {
        w->appendOutput(std::to_string(source));
}
void Printer::prints(const uint32_t *source, uint8_t words,MainWindow *w) {
    for (uint8_t i = 0; i < words; i++) {
        char *ptr = (char*)source+i;
        char *msg = new char[4];
        for(int i=0;i<4;++i, ++ptr)
            msg[3-i] = *ptr;
        int c;
        std::string buffer="";
        c=0;
        while(c<4) {
          buffer.append(1,msg[c]);
          ++c;
        }
        w->appendOutput(std::string(msg));
    }
}
void Printer::prints(const uint32_t source,MainWindow *w) {
        char *ptr = (char*)&source;
        char *msg = new char[4];
        for(int i=0;i<4;++i, ++ptr)
            msg[3-i] = *ptr;
        int c;
        std::string buffer="";
        c=0;
        while(c<4) {
          buffer.append(1,msg[c]);
          ++c;
        }
        w->appendOutput(std::string(msg));
}
