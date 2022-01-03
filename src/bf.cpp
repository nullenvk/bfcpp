#include "bf.h"
#include <cstdio>
    
VM::VM(const std::vector<InstrType> code, size_t memSize) : code(code) {
    this->mem = new uint8_t[memSize];
}

VM::~VM() {
    delete this->mem;
}

void VM::mpMove(int size) {
    mp += size;

    if(mp < 0)
        mp = memSize + mp;
    else if(mp >= memSize)
        mp = mp % memSize;
}

void VM::loopL() {
    if(mem[mp] == 0) {
        int depth = 1;

        cp++;
        while(depth != 0) {
            if(code[cp] == INSTR_LOOPL) depth++;
            else if(code[cp] == INSTR_LOOPR) depth--;
            cp++;

            if(cp >= code.size() || cp < 0)
                return;
        }

    } else {
        cp++;
    }
}

void VM::loopR() {
    if(mem[mp] == 0) {
        cp++;
        return;
    }

    int depth = 1;
    while(depth != 0) {
        cp--;
        if(code[cp] == INSTR_LOOPL) depth--;
        else if(code[cp] == INSTR_LOOPR) depth++;
            
        if(cp >= code.size() || cp < 0)
            return;
    }
}

void VM::run() {
    while(1) {
        switch(code[cp]) {
            case INSTR_LEFT:
                mpMove(-1);
                cp++;
                break;

            case INSTR_RIGHT:
                mpMove(1);
                cp++;
                break;

            case INSTR_PLUS:
                mem[mp]++;
                cp++;
                break;
            
            case INSTR_MINUS:
                mem[mp]--;
                cp++;
                break;

            case INSTR_WRITE:
                putchar(mem[mp]);
                cp++;
                break;

            case INSTR_READ:
                mem[mp] = getchar();
                cp++;
                break;

            case INSTR_LOOPL:
                loopL();
                break;
            
            case INSTR_LOOPR:
                loopR();
                break;
        }

        if(cp >= code.size() || cp < 0)
            break;
    }
}
