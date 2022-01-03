#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <string_view>

#include "bf.h"

const char *PROG_NAME;

void usage() {
    std::cerr << PROG_NAME << " [-m memory_cells] filename" << std::endl;
    exit(1);
}

int charToInstrType(char c, InstrType& t) {
    switch(c) {
        case '<': t = INSTR_LEFT; break;
        case '>': t = INSTR_RIGHT; break;
        case '+': t = INSTR_PLUS; break;
        case '-': t = INSTR_MINUS; break;
        case '.': t = INSTR_WRITE; break;
        case ',': t = INSTR_READ; break;
        case '[': t = INSTR_LOOPL; break;
        case ']': t = INSTR_LOOPR; break;
        default: return 1;
    }

    return 0;
}

int readFile(const char *filename, std::vector<InstrType>& data) {
    char ch;
    InstrType type;

    std::fstream fin(filename, std::fstream::in);
    while(fin >> std::noskipws >> ch)
        if(charToInstrType(ch, type) == 0)
            data.push_back(type);

    return 0;
}

bool isNumber(std::string_view str) {
    for(char c : str)
        if(!std::isdigit(c)) return false;

    return true;
}

int main(int argc, char **argv) {
    size_t memSize = 30000; // Cell count
    std::vector<InstrType> instrs;

    PROG_NAME = argv[0];

    for(;;) {
        switch(getopt(argc, argv, "m:")) {
            case 'm':
                if(!isNumber(optarg))
                    usage();
                
                memSize = atoll(optarg);
                continue;

            default:
                usage();
                break;

            case -1:
                break;
        }

        break;
    }

    if(optind != argc - 1)
        usage();

    readFile(argv[optind], instrs);

    VM vm(instrs, memSize);
    vm.run();
}
