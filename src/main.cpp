#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <fstream>
#include <string_view>

#include "bf.h"
#include "ir.h"
#include "vm.h"

#include "asmgen_x64.h"

const char *PROG_NAME;

void usage() {
    std::cerr << PROG_NAME << " [-m memory_cells] filename" << std::endl;
    exit(1);
}

int readFile(const char *filename, std::vector<BF::Instr>& data) {
    std::filebuf f;
    if(!f.open(filename, std::ios::in))
        return 1;

    std::istream str(&f);
    BF::parseStream(str, data);

    return 0;
}

bool isNumber(std::string_view str) {
    for(char c : str)
        if(!std::isdigit(c)) return false;

    return true;
}

int main(int argc, char **argv) {
    size_t memSize = 30000; // Cell count
    std::vector<BF::Instr> instrs;

    PROG_NAME = argv[0];
    std::string outFile("out.s");

    for(;;) {
        switch(getopt(argc, argv, "m:o:")) {
            case 'm':
                if(!isNumber(optarg))
                    usage();
                
                memSize = atoll(optarg);
                continue;

            case 'o':
                outFile = std::string(optarg);
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

    if(readFile(argv[optind], instrs)) {
        std::cerr << "Fatal error: Cannot open the file" << std::endl;
        usage();
    }
    
    IR::Tree ir;
    ir.parseBF(instrs);
    
    std::ofstream outf(outFile);
    std::streambuf *outs(outf.rdbuf());
    std::ostream outo(outs);
    
    AsmGen_x64 gen(memSize);
    gen.gen(outo, ir);
}
