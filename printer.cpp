//
// Created by elsis on 18.4.17.
//

#include <iostream>
#include "printer.h"

void Printer::print(const uint32_t *source, uint8_t words) {
    for (uint8_t i = 0; i < words; i++) {
        std::cout << *(source + i) << std::endl;
    }
}
