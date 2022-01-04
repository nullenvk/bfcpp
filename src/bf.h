#pragma once
#include <vector>
#include <cstdint>
#include <stack>
#include <istream>
#include <vector>

namespace BF {
    enum Instr {
        LEFT,
        RIGHT,
        PLUS,
        MINUS,
        WRITE,
        READ,
        LOOPL,
        LOOPR,
    };

    void parseStream(std::istream& str, std::vector<Instr>& data);

    class VM {
        const std::vector<BF::Instr> code;
        uint8_t *mem;
        size_t memSize;

        size_t cp = 0, mp = 0; // Code pointer, Memory pointer

        void mpMove(int side); // -1 - left, 1 - right
        void loopL();
        void loopR();
    public:
        VM(const std::vector<BF::Instr> code, size_t memSize);
        ~VM();
        void run();
    };
}

