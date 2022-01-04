#pragma once
#include <vector>

#include "parser.h"

namespace IR {
    struct Atom;

    struct Tree {
        std::vector<Atom> atoms;

        void parseBF(std::vector<BF::Instr>);
    };

    struct Instr  {
        enum Type {
            ADD,
            MOVE,
            READ,
            WRITE,
        };

        int param; // for ADD, MOVE
    };

    struct Atom {
        enum Type {
            LOOP,
            INSTR
        };

        union {
            Instr instr;
            Tree loop;
        };
    };
}
