//
// Created by elsis on 18.4.25.
//

#include "parsed_program.h"

ParsedProgram::~ParsedProgram() {
    delete name;
    delete code;
    delete data;
}

void ParsedProgram::setLines(uint32_t lines) {
    this->lines = lines;
}

void ParsedProgram::setName(uint32_t *name, uint16_t size) {
    this->name = name;
    this->nameSize = size;
}

void ParsedProgram::setCode(uint32_t *code, uint16_t size) {
    this->code = code;
    this->codeSize = size;
}

void ParsedProgram::setData(uint32_t *data, uint16_t size) {
    this->data = data;
    this->dataSize = size;
}

uint32_t ParsedProgram::getLines() const {
    return lines;
}

uint32_t *ParsedProgram::getName() const {
    return name;
}

uint16_t ParsedProgram::getNameSize() const {
    return nameSize;
}

uint32_t *ParsedProgram::getCode() const {
    return code;
}

uint16_t ParsedProgram::getCodeSize() const {
    return codeSize;
}

uint32_t *ParsedProgram::getData() const {
    return data;
}

uint16_t ParsedProgram::getDataSize() const {
    return dataSize;
}
