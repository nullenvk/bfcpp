#include "asmgen_x64.h"

#include <sstream>

// This generator assumes NASM syntax
// bfmain accepts cell address in rax

typedef std::deque< std::pair<const IR::Tree*, size_t> > RecursionStack; // size_t - position

void genPrologue(std::ostream& out) {
    out << "SECTION .text" << std::endl
        << "global bfmain" << std::endl
        << "bfmain:"       << std::endl;
}

void genEpilogue(std::ostream& out) {
    out << "ret" << std::endl;
}

std::string recursionToLoopName(RecursionStack rec) {
    std::stringstream ns;

    ns << "loop";

    while(!rec.empty()) {
        ns << "_" << std::to_string(rec.front().second);
        rec.pop_front();
    }

    return ns.str();
}

void genStartLoop(std::ostream& out, RecursionStack rec) {
    out << recursionToLoopName(rec) << ":" << std::endl;
}

void genEndLoop(std::ostream& out, RecursionStack rec) {
    out << "loop " << recursionToLoopName(rec) << std::endl;
}

void genAdd(std::ostream& out, int param) {
    out << "add rcx, " << param << std::endl;
}

void genMove(std::ostream& out, int param) {
    out << "mov [rsi], rcx" << std::endl
        << "add rsi, " << param << std::endl
        << "mov rcx, [rsi]" << std::endl;
}

void genClear(std::ostream& out) {
    out << "xor rcx, rcx" << std::endl;
}

void genInstruction(std::ostream& out, IR::Instr instr) {
    switch(instr.type) {
        case IR::Instr::ADD:
            genAdd(out, instr.param);
            break;

        case IR::Instr::MOVE:
            genMove(out, instr.param);
            break;

        case IR::Instr::CLEAR:
            genClear(out);
            break;

        case IR::Instr::READ:
        case IR::Instr::WRITE:
            // TODO
            break;

        case IR::Instr::_INVALID: break;
    }
}

void AsmGen_x64::gen(std::ostream& out, const IR::Tree ir) {
    genPrologue(out);

    // Tree recursion
    RecursionStack recursion;
    recursion.push_back(std::pair(&ir, 0));
    
    while(!recursion.empty()) {
        const IR::Tree *node = recursion.back().first;
        size_t& pos = recursion.back().second;

        if(pos >= node->atoms.size()) {
            recursion.pop_back();

            if(!recursion.empty()) {
                genEndLoop(out, recursion);
                recursion.back().second++;
            }

            continue;
        }

        if(node->atoms[pos].type == IR::Atom::INSTR) {
            genInstruction(out, node->atoms[pos].instr);
            pos++;
        } else {
            genStartLoop(out, recursion);
            recursion.push_back(std::pair(&node->atoms[pos].loop, 0));
        }
    }

    genEpilogue(out);
}
