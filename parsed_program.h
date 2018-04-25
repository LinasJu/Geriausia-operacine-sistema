//
// Created by elsis on 18.4.25.
//

#ifndef BABYOS_PARSEDPROGRAM_H
#define BABYOS_PARSEDPROGRAM_H

#include <cstdint>

class ParsedProgram {
public:
    ~ParsedProgram();

    void setLines(uint32_t lines);

    void setName(uint32_t *name, uint16_t size);

    void setCode(uint32_t *code, uint16_t size);

    void setData(uint32_t *data, uint16_t size);

    uint32_t getLines() const;

    uint32_t *getName() const;

    uint16_t getNameSize() const;

    uint32_t *getCode() const;

    uint16_t getCodeSize() const;

    uint32_t *getData() const;

    uint16_t getDataSize() const;

private:
    uint32_t lines;
    uint32_t *name;
    uint16_t nameSize;
    uint32_t *code;
    uint16_t codeSize;
    uint32_t *data;
    uint16_t dataSize;
};


#endif //BABYOS_PARSEDPROGRAM_H
