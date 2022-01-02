#include "bf.h"
    
VM::VM(const std::vector<InstrType> code, size_t memSize) : code(code) {
    this->mem = new uint8_t[memSize];
}

VM::~VM() {
    delete this->mem;
}

void VM::run() {
    while(1) {
    }
}
