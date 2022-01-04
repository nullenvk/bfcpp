#include "parser.h"

int charToInstr(char c, BF_Instr& t) {
    switch(c) {
        case '<': t = BF_LEFT; break;
        case '>': t = BF_RIGHT; break;
        case '+': t = BF_PLUS; break;
        case '-': t = BF_MINUS; break;
        case '.': t = BF_WRITE; break;
        case ',': t = BF_READ; break;
        case '[': t = BF_LOOPL; break;
        case ']': t = BF_LOOPR; break;
        default: return 1;
    }

    return 0;
}

void BF_parseStream(std::istream& str, std::vector<BF_Instr>& data) {
    char ch;
    BF_Instr type;

    while(str >> std::noskipws >> ch)
        if(charToInstr(ch, type) == 0)
            data.push_back(type);
}
