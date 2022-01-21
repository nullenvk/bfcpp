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

// Remove Later
void irDebugPrint(const IR::Tree ir, int depth) {
    for(auto a : ir.atoms) {
        for(int i = 0; i < depth; i++)
            std::cout << " ";
        
        if(a.type == IR::Atom::INSTR) {
            switch(a.instr.type) {
                case IR::Instr::ADD:
                case IR::Instr::MOVE:
                    std::cout << (a.instr.type == IR::Instr::ADD ? "Add " : "Move ")  << a.instr.param << std::endl;
                    break;

                case IR::Instr::WRITE:
                case IR::Instr::READ:
                    std::cout << (a.instr.type == IR::Instr::WRITE ? "Write " : "Read ") << std::endl;
                    break;

                case IR::Instr::_INVALID:
                    std::cout << "???" << std::endl;
                    break;
            }
        } else {
            std::cout << "[LOOP]" << std::endl;
            irDebugPrint(a.loop, depth + 4);
            std::cout << "[END OF LOOP]" << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    size_t memSize = 30000; // Cell count
    std::vector<BF::Instr> instrs;

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
    
    IR::Tree ir;
    ir.parseBF(instrs);
    
    std::ofstream outf("out.s");
    std::streambuf *outs(outf.rdbuf());
    std::ostream outo(outs);
    
    AsmGen_x64 gen(memSize);
    gen.gen(outo, ir);
}
