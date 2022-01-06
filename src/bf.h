#pragma once
#include <vector>
#include <cstdint>
#include <stack>
#include <istream>
#include <vector>

namespace BF {
    enum Instr {
        LEFT,
        RIGHT,
        PLUS,
        MINUS,
        WRITE,
        READ,
        LOOPL,
        LOOPR,
    };

    void parseStream(std::istream& str, std::vector<Instr>& data);
}

