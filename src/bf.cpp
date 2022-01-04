#include "bf.h"
#include <cstdio>
    
BFVM::BFVM(const std::vector<BF::Instr> code, size_t memSize) : code(code) {
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
            if(code[cp] == BF::LOOPL) depth++;
            else if(code[cp] == BF::LOOPR) depth--;
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
        if(code[cp] == BF::LOOPL) depth--;
        else if(code[cp] == BF::LOOPR) depth++;
            
        if(cp >= code.size() || cp < 0)
            return;
    }
}

void BFVM::run() {
    while(1) {
        switch(code[cp]) {
            case BF::LEFT:
                mpMove(-1);
                cp++;
                break;

            case BF::RIGHT:
                mpMove(1);
                cp++;
                break;

            case BF::PLUS:
                mem[mp]++;
                cp++;
                break;
            
            case BF::MINUS:
                mem[mp]--;
                cp++;
                break;

            case BF::WRITE:
                putchar(mem[mp]);
                cp++;
                break;

            case BF::READ:
                mem[mp] = getchar();
                cp++;
                break;

            case BF::LOOPL:
                loopL();
                break;
            
            case BF::LOOPR:
                loopR();
                break;
        }

        if(cp >= code.size() || cp < 0)
            break;
    }
}
