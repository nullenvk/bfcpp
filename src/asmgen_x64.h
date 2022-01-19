#pragma once

#include "asmgen.h"

class AsmGen_x64 : public AsmGen {
public:
    void gen(std::ostream& out, const IR::Tree ir);
};
