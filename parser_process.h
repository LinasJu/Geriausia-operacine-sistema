//
// Created by elsis on 18.4.23.
//

#ifndef BABYOS_PARSERPROCESS_H
#define BABYOS_PARSERPROCESS_H

#include <cstdint>
#include <string>

#include "parsed_program.h"

class ParserProcess {
public:
    enum class State {
        NONE,
        AMJ,
        COD,
        DAT,
        END,
        ERROR
    };

    ParserProcess(uint32_t *source, uint16_t size);
    void parse();
    bool isError();
    ParsedProgram *getParsedProgram();
    std::string &getParseLog();

private:

    std::string commandToString(uint32_t command);
    bool isNumber(const std::string &s);
    uint16_t handleSegment(uint16_t index, uint16_t maxLength, std::string endWord);
    void copyMemory(uint32_t *dest, uint16_t start, uint16_t size);

    State currentState = State::NONE;
    std::string parseLog;
    uint32_t *source;
    uint16_t size;
    ParsedProgram *program;
};

#endif //BABYOS_PARSERPROCESS_H
