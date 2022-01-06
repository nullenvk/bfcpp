#include "bf.h"
#include <cstdio>

using namespace BF;

int charToInstr(char c, Instr& t) {
    switch(c) {
        case '<': t = LEFT; break;
        case '>': t = RIGHT; break;
        case '+': t = PLUS; break;
        case '-': t = MINUS; break;
        case '.': t = WRITE; break;
        case ',': t = READ; break;
        case '[': t = LOOPL; break;
        case ']': t = LOOPR; break;
        default: return 1;
    }

    return 0;
}

void BF::parseStream(std::istream& str, std::vector<Instr>& data) {
    char ch;
    Instr type;

    while(str >> std::noskipws >> ch)
        if(charToInstr(ch, type) == 0)
            data.push_back(type);
}
