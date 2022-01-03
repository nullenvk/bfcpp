#include "bf.h"
#include <cstdio>
    
VM::VM(const std::vector<BF_Instr> code, size_t memSize) : code(code) {
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
            if(code[cp] == BF_LOOPL) depth++;
            else if(code[cp] == BF_LOOPR) depth--;
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
        if(code[cp] == BF_LOOPL) depth--;
        else if(code[cp] == BF_LOOPR) depth++;
            
        if(cp >= code.size() || cp < 0)
            return;
    }
}

void VM::run() {
    while(1) {
        switch(code[cp]) {
            case BF_LEFT:
                mpMove(-1);
                cp++;
                break;

            case BF_RIGHT:
                mpMove(1);
                cp++;
                break;

            case BF_PLUS:
                mem[mp]++;
                cp++;
                break;
            
            case BF_MINUS:
                mem[mp]--;
                cp++;
                break;

            case BF_WRITE:
                putchar(mem[mp]);
                cp++;
                break;

            case BF_READ:
                mem[mp] = getchar();
                cp++;
                break;

            case BF_LOOPL:
                loopL();
                break;
            
            case BF_LOOPR:
                loopR();
                break;
        }

        if(cp >= code.size() || cp < 0)
            break;
    }
}
