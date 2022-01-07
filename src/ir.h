#pragma once
#include <vector>

#include "bf.h"

namespace IR {
    struct Atom;

    class Tree {
        bool isEquivToClear();
        void optimizeClear();
    public:
        std::vector<Atom> atoms;

        void parseBF(std::vector<BF::Instr>);
    };

    struct Instr  {
        enum Type {
            ADD,
            MOVE,
            READ,
            WRITE,
            CLEAR,

            _INVALID
        } type;

        int param; // for ADD, MOVE

        Instr();
        Instr(Type t);
        Instr(Type t, int param);
    };

    struct Atom {
        enum Type {
            LOOP,
            INSTR
        } type;

        Instr instr;
        Tree loop;

        Atom(Instr instr);
        Atom(Tree loop);
        ~Atom();

        void clean();
    };
}
