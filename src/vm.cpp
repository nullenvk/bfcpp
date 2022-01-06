#include "vm.h"
#include <iostream>
#include <unistd.h>
    
VM::VM(const IR::Tree code, size_t memSize) : code(code) {
    this->mem = new uint8_t[memSize];
    this->memSize = memSize;
}

VM::~VM() {
    delete this->mem;
}

void VM::mpAdd(int arg) {
    mp += arg;

    if(mp < 0)
        mp += (int)memSize;

    mp = mp % memSize;
}

void VM::run() {
    std::stack<std::pair<const IR::Tree*, int> > recursions;
    recursions.push(std::pair(&this->code, 0));

    while(!recursions.empty()) {
        const IR::Tree *node = recursions.top().first;
        int& pos = recursions.top().second;

        if(pos >= node->atoms.size()) {
            recursions.pop();
            continue;
        }

        // INSTR
        if(node->atoms[pos].type == IR::Atom::INSTR) {
            IR::Instr instr = node->atoms[pos].instr;

            switch(instr.type) {
                case IR::Instr::ADD:    mem[mp] += instr.param; break;
                case IR::Instr::MOVE:   mpAdd(instr.param); break;
                case IR::Instr::READ:   mem[mp] = getchar(); break;
                case IR::Instr::WRITE:  putchar(mem[mp]); break;

                case IR::Instr::_INVALID: break; // Maybe add warning messages?
            }

            pos++;
        } 
        // LOOP
        else {
            if(mem[mp] == 0)
                pos++;
            else
                recursions.push(std::pair(&node->atoms[pos].loop, 0));
        }
    }
}
