#include "asmgen_x64.h"

#include <sstream>

// This generator assumes NASM syntax and Linux OS

typedef std::deque< std::pair<const IR::Tree*, size_t> > RecursionStack; // size_t - position

void genPrologue(std::ostream& out, size_t memSize) {
    out << "global _start" << std::endl

        << "SECTION .bss" << std::endl
        << "bfbuff resq " << memSize << std::endl

        << "SECTION .text" << std::endl

        << "bfwrite:" << std::endl
        << "push rsi" << std::endl
        << "push rcx" << std::endl
        << "mov [rsp-8], rcx" << std::endl
        << "mov rax, 1" << std::endl
        << "mov rdi, 1" << std::endl
        << "mov rsi, rsp" << std::endl
        << "sub rsi, 8" << std::endl
        << "mov rdx, 1" << std::endl
        << "syscall" << std::endl
        << "pop rcx" << std::endl
        << "pop rsi" << std::endl
        << "ret" << std::endl
        
        << "_start:" << std::endl

        // Initialize bfbuff
        << "mov rcx, " << memSize << std::endl
        << "xor rax, rax" << std::endl
        << "bfbuff_init: " << std::endl
        << "mov [bfbuff + (rcx - 1) * 8], rax" << std::endl
        << "loop bfbuff_init" << std::endl
        << "" << std::endl

        << "mov rcx, 0" << std::endl
        << "mov rsi, bfbuff" << std::endl;
}

void genEpilogue(std::ostream& out) {
    out << "mov rax, 60" << std::endl
        << "xor rdi, rdi" << std::endl
        << "syscall" << std::endl;
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
    out << "test rcx, rcx" << std::endl
        << "jnz " << recursionToLoopName(rec) << std::endl;
}

void genAdd(std::ostream& out, int param) {
    out << "add rcx, " << param << std::endl;
}

void genMove(std::ostream& out, int param) {
    out << "mov [rsi], rcx" << std::endl
        << "add rsi, " << param * 8 << std::endl
        << "mov rcx, [rsi]" << std::endl;
}

void genClear(std::ostream& out) {
    out << "xor rcx, rcx" << std::endl;
}

void genWrite(std::ostream& out) {
    out << "call bfwrite" << std::endl;
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

        case IR::Instr::WRITE:
            genWrite(out);
            break;

        case IR::Instr::READ:
            // TODO
            break;

        case IR::Instr::_INVALID: break;
    }
}

void AsmGen_x64::gen(std::ostream& out, const IR::Tree ir) {
    genPrologue(out, memSize);

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
