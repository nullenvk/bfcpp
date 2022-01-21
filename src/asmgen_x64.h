#pragma once

#include "asmgen.h"

class AsmGen_x64 : public AsmGen {
private:
    size_t memSize;
public:
    void gen(std::ostream& out, const IR::Tree ir);
    AsmGen_x64(size_t memSize) : memSize(memSize) {}
};
