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

void readStream(std::istream& str, std::vector<BF_Instr>& data) {
    char ch;
    BF_Instr type;

    while(str >> std::noskipws >> ch)
        if(charToInstr(ch, type) == 0)
            data.push_back(type);
}

int readFile(const char *filename, std::vector<BF_Instr>& data) {
    std::filebuf f;
    if(!f.open(filename, std::ios::in))
        return 1;

    std::istream str(&f);
    readStream(str, data);

    return 0;
}

bool isNumber(std::string_view str) {
    for(char c : str)
        if(!std::isdigit(c)) return false;

    return true;
}

int main(int argc, char **argv) {
    size_t memSize = 30000; // Cell count
    std::vector<BF_Instr> instrs;

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
