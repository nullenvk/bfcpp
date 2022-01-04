#include "bf.h"
#include <cstdio>
    
BFVM::BFVM(const std::vector<BF_Instr> code, size_t memSize) : code(code) {
    this->mem = new uint8_t[memSize];
}

BFVM::~BFVM() {
    delete this->mem;
}

void BFVM::mpMove(int size) {
    mp += size;

    if(mp < 0)
        mp = memSize + mp;
    else if(mp >= memSize)
        mp = mp % memSize;
}

void BFVM::loopL() {
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

void BFVM::loopR() {
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

void BFVM::run() {
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
