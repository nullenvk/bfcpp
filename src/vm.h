#pragma once

#include "ir.h"

class VM {
    const IR::Tree code;
    uint8_t *mem;
    size_t memSize;

    int cp = 0, mp = 0; // Code pointer, Memory pointer

    void mpAdd(int p); 
public:
    VM(const IR::Tree code, size_t memSize);
    ~VM();
    void run();
};
