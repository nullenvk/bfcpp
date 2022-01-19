#pragma once

#include "ir.h"

class AsmGen {
public:
    virtual void gen(std::ostream& out, const IR::Tree ir) = 0;
};
