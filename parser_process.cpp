//
// Created by elsis on 18.4.23.
//

#include <algorithm>
#include <cstring>
#include "parser_process.h"

ParserProcess::ParserProcess(uint32_t *source, uint16_t size) {
    this->source = source;
    this->size = size;
}

void ParserProcess::parse() {
    uint16_t current = 0;
    uint16_t segmentLength = 0;
    program = new ParsedProgram;

    while (true) {
        std::string command = commandToString(source[current]);
        if (command.find("$AMJ") != std::string::npos) {
            if (currentState != State::NONE) {
                currentState = State::ERROR;
                break;
            }
            currentState = State::AMJ;

            if (++current >= size) {
                parseLog = "Memory limit exceeded before end tag";
                currentState = State::ERROR;
                break;
            }
            command = commandToString(source[current]);
            if (!isNumber(command)) {
                parseLog = "Invalid number of lines";
                currentState = State::ERROR;
                break;
            }

            program->setLines(source[current]);
            //program->lines = source[current];

            if (++current >= size) {
                parseLog = "Memory limit exceeded before end tag";
                currentState = State::ERROR;
                break;
            }

            segmentLength = handleSegment(current, 8, "$COD");

            if (currentState == State::ERROR) {
                break;
            }

            //program->setCode();
            auto buffer = new uint32_t[segmentLength];
            copyMemory(buffer, current, segmentLength);
            current += segmentLength;
            program->setName(buffer, segmentLength);

            continue;
        }
        else if (command.find("$COD") != std::string::npos) {
            if (currentState != State::AMJ) {
                currentState = State::ERROR;
                break;
            }
            currentState = State::COD;

            if (++current >= size) {
                parseLog = "Memory limit exceeded before end tag";
                currentState = State::ERROR;
                break;
            }

            segmentLength = handleSegment(current, 100, "$DAT");

            if (currentState == State::ERROR) {
                break;
            }

            auto buffer = new uint32_t[segmentLength];
            copyMemory(buffer, current, segmentLength);
            current += segmentLength;
            program->setCode(buffer, segmentLength);

            continue;
        }
        else if (command.find("$DAT") != std::string::npos) {
            if (currentState != State::COD) {
                currentState = State::ERROR;
                break;
            }
            currentState = State::DAT;

            if (++current >= size) {
                parseLog = "Memory limit exceeded before end tag";
                currentState = State::ERROR;
                break;
            }

            segmentLength = handleSegment(current, 99, "$END");
            if (currentState == State::ERROR) {
                break;
            }

            auto buffer = new uint32_t[segmentLength];
            copyMemory(buffer, current, segmentLength);
            current += segmentLength;
            program->setData(buffer, segmentLength);

            continue;
        }
        else if (command.find("$END") != std::string::npos) {
            if (currentState != State::DAT) {
                currentState = State::ERROR;
                break;
            }
            currentState = State::END;
            // END state reached
            break;
        }
    }

    if (currentState == State::END && current+1 < size) {
        // error, lines after end
        parseLog = "Program not finished after $END";
        currentState = State::ERROR;
    }

    if (currentState == State::ERROR) {
        parseLog + " @ line " + std::to_string(current);
        delete program;
    }
}

std::string ParserProcess::commandToString(uint32_t command) {
    char word[4];
    for (int i = 0; i < 4; i++) {
        word[i] = (char)((command >> (24 - 8*i)) & 0xFF);
    }

    return std::string(word);
}

bool ParserProcess::isNumber(const std::string &s) {
    return !s.empty() && std::find_if(s.begin(), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

uint16_t ParserProcess::handleSegment(uint16_t index, uint16_t maxLength, std::string endWord) {
    std::string command;
    for (uint16_t i = 0; i < maxLength; i++) {
        command = commandToString(source[index]);
        if (command == endWord) {
            if (i == 0) {
                // error, empty segment
                parseLog = "Empty segment before " + endWord;
                currentState = State::ERROR;
            }
            return i;
        }
        // name
        if (++index >= size) {
            currentState = State::ERROR;
            return i;
        }
    }

    command = commandToString(source[index]);
    if (!(command == endWord)) {
        // no end word
        parseLog = endWord + " tag is not in correct place";
        currentState = State::ERROR;
    }

    return maxLength;
}

void ParserProcess::copyMemory(uint32_t *dest, uint16_t start, uint16_t size) {
    for (int i = 0; i < size; i++) {
        dest[i] = source[start + i];
    }
}

bool ParserProcess::isError() {
    return currentState == State::ERROR;
}

ParsedProgram *ParserProcess::getParsedProgram() {
    return program;
}

std::string &ParserProcess::getParseLog() {
    return parseLog;
}
