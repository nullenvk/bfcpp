#include "asmgen_x64.h"

// This generator assumes NASM syntax
// bfmain accepts cell address in rax

void genPrologue(std::ostream& out) {
    out << "SECTION .text" << std::endl
        << "global bfmain" << std::endl
        << "bfmain:"       << std::endl;
}

void genEpilogue(std::ostream& out) {
    out << "    ret" << std::endl;
}

void AsmGen_x64::gen(std::ostream& out, const IR::Tree ir) {
    genPrologue(out);

    // Tree recursion
    std::stack< std::pair<const IR::Tree*, size_t> > recursion;
    recursion.push(std::pair(&ir, 0));
    
    while(!recursion.empty()) {
        const IR::Tree *node = recursion.top().first;
        size_t& pos = recursion.top().second;

        if(pos >= node->atoms.size()) {
            recursion.pop();
            if(!recursion.empty())
                recursion.top().second++;
            continue;
        }

        if(node->atoms[pos].type == IR::Atom::INSTR)
            pos++;
        else
            recursion.push(std::pair(&node->atoms[pos].loop, 0));
    }

    genEpilogue(out);
}
