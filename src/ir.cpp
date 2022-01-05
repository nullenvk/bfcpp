#include "ir.h"
#include "bf.h"
#include <stack>
#include <iterator>

using namespace IR;

Instr::Instr() {
    type = _INVALID;
}

Instr::Instr(Type t) {
    type = t;
}

Instr::Instr(Type t, int p) {
    type = t;
    param = p;
}

Atom::Atom(Instr instr) {
    this->type = Atom::INSTR;
    this->instr = instr;
}

Atom::Atom(Tree loop) {
    this->type = Atom::LOOP;
    this->loop = loop;
}

Atom::~Atom() {

}

std::pair<size_t, int> instrSubstrOptimize(std::vector<BF::Instr>::iterator it) {
    size_t len = 0;
    int sum = 0;
    Instr::Type type;

    if(*it == BF::LEFT || *it == BF::RIGHT)
        type = Instr::MOVE;
    else if(*it == BF::PLUS || *it == BF::MINUS)
        type = Instr::ADD;
    else
        return std::pair(1, 0);
    
    BF::Instr instr_plus, instr_minus;
    instr_plus = (type == Instr::ADD ? BF::PLUS : BF::RIGHT);
    instr_minus = (type == Instr::ADD ? BF::MINUS : BF::LEFT);

    while(*it == instr_plus || *it == instr_minus) {
        sum += *it == instr_plus ? 1 : -1;
        len++;
        it++;
    }

    return std::pair(len, sum);
}

void Tree::parseBF(std::vector<BF::Instr> bfi) {
    std::stack<Tree*> recursion;
    std::vector<BF::Instr>::iterator it = bfi.begin();

    recursion.push(this);

    while(it != bfi.end()) {
        Tree *node = recursion.top();

        std::pair<size_t, int> optimizeRes = instrSubstrOptimize(it);
        switch(*it) {
            case BF::LEFT:
            case BF::RIGHT:
                node->atoms.push_back(Instr(IR::Instr::MOVE, optimizeRes.second));
                it += optimizeRes.first;
                break;
                
            case BF::PLUS:
            case BF::MINUS:
                node->atoms.push_back(Instr(IR::Instr::ADD, optimizeRes.second));
                it += optimizeRes.first;
                break;

            case BF::READ:
                node->atoms.push_back(IR::Instr(IR::Instr::READ));
                it++;
                break;
            
            case BF::WRITE:
                node->atoms.push_back(IR::Instr(IR::Instr::WRITE));
                it++;
                break;

            case BF::LOOPL:
                node->atoms.push_back(IR::Tree());
                recursion.push(&node->atoms.back().loop);
                it++;
                break;

            case BF::LOOPR:
                recursion.pop();
                it++;
                break;
        }
    }
}
