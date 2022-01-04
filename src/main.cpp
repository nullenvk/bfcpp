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

int readFile(const char *filename, std::vector<BF_Instr>& data) {
    std::filebuf f;
    if(!f.open(filename, std::ios::in))
        return 1;

    std::istream str(&f);
    BF_parseStream(str, data);

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
