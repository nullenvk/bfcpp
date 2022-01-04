#pragma once
#include <vector>
#include <cstdint>
#include <stack>

#include "parser.h"

class BFVM {
    const std::vector<BF::Instr> code;
    uint8_t *mem;
    size_t memSize;

    size_t cp = 0, mp = 0; // Code pointer, Memory pointer

    void mpMove(int side); // -1 - left, 1 - right
    void loopL();
    void loopR();
public:
    BFVM(const std::vector<BF::Instr> code, size_t memSize);
    ~BFVM();
    void run();
};
