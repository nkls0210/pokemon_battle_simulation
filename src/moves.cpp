#include "moves.h"

Move::Move(): attackName("???"), moveType(Type()), strength(0), accuracy(0), physicalMove(true){}

Move::Move(string n, string t, unsigned d, unsigned acc, bool phys):
    attackName(n), moveType(Type(t)), strength(d), accuracy(acc), physicalMove(phys){}
    
unsigned Move::moveEffect(){
        return 0;
    }

StatusMove::StatusMove(): Move(){}

StatusMove::StatusMove(string n, string t, unsigned s, unsigned acc, bool phys, unsigned scv, unsigned c): Move(n,t,s,acc,phys), statusCondValue(scv), chance(c){}

unsigned StatusMove::moveEffect(){
        unsigned randVal = rand() % 100;
        if(randVal < chance){
            return statusCondValue;
        }
        return 0;
    }
