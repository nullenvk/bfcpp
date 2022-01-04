#pragma once
#include <istream>
#include <vector>

enum BF_Instr{
    BF_LEFT,
    BF_RIGHT,
    BF_PLUS,
    BF_MINUS,
    BF_WRITE,
    BF_READ,
    BF_LOOPL,
    BF_LOOPR,
};

void BF_parseStream(std::istream& str, std::vector<BF_Instr>& data);
