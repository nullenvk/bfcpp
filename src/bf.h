#pragma once
#include <vector>
#include <cstdint>
#include <stack>

enum InstrType {
    INSTR_LEFT,
    INSTR_RIGHT,
    INSTR_PLUS,
    INSTR_MINUS,
    INSTR_WRITE,
    INSTR_READ,
    INSTR_LOOPL,
    INSTR_LOOPR,
};

class VM {
    const std::vector<InstrType> code;
    uint8_t *mem;
    size_t memSize;

    size_t cp = 0, mp = 0; // Code pointer, Memory pointer

    void mpMove(int side); // -1 - left, 1 - right
    void loopL();
    void loopR();
public:
    VM(const std::vector<InstrType> code, size_t memSize);
    ~VM();
    void run();
};
