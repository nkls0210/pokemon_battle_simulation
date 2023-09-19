#include "moves.h"

Move::Move(): attackName("???"), moveType(Type()), strength(0), accuracy(0), physicalMove(true){}
Move::Move(string n, string t, unsigned d, unsigned acc, bool phys):
    attackName(n), moveType(Type(t)), strength(d), accuracy(acc), physicalMove(phys){}
unsigned Move::moveEffect(){
        return 0;
    }
unsigned Move::moveHealing(unsigned buf){
    buf = 0;
    return buf;
};

StatusMove::StatusMove(): Move(){}
StatusMove::StatusMove(string n, string t, unsigned s, unsigned acc, bool phys, unsigned scv, unsigned c): Move(n,t,s,acc,phys), statusCondValue(scv), chance(c){}
unsigned StatusMove::moveEffect(){
        unsigned randVal = rand() % 100;
        if(randVal < chance){
            return statusCondValue;
        }
        return 0;
    }
unsigned StatusMove::moveHealing(unsigned buf){
    buf = 0;
    return buf;
};

HealingMove::HealingMove(): Move(), healingPrecent(0){}
HealingMove::HealingMove(string n, string t, unsigned s, unsigned acc, bool phys, unsigned heal): Move(n,t,s,acc,phys), healingPrecent(heal){};
unsigned HealingMove::moveEffect(){
    return 0;
}
unsigned HealingMove::moveHealing(unsigned damage){
    return (damage*healingPrecent/100);
}

BoostMove::BoostMove():Move(){
    for(unsigned i = 0; i < 5; i++){
        statBoosts[i]= 0;
    }
}
BoostMove::BoostMove(string n, string t, unsigned s, unsigned acc, bool phys, vector<int> statChange): Move(n,t,s,acc,phys){
    for(unsigned i = 0; i < statChange.size(); i++){
        statBoosts[i]= statChange[i];
    }
};
unsigned BoostMove::moveEffect(){
    return 0;
};
unsigned BoostMove::moveHealing(unsigned buf){
    buf = 0;
    return buf;
};


Move* operator>>(istream& in, Move*& move){
    string name;
    string type;
    unsigned strength;
    unsigned accuracy;
    bool isPhys;

    in >> name;
    in >> type;
    in >> strength;
    in >> accuracy;
    in >> isPhys;

    move = new Move(name,type,strength,accuracy,isPhys);
    return move;
}

StatusMove* operator>>(istream& in, StatusMove*& move){
    string name;
    string type;
    unsigned strength;
    unsigned accuracy;
    bool isPhys;
    string cond;
    unsigned condChance;

    in >> name;
    in >> type;
    in >> strength;
    in >> accuracy;
    in >> isPhys;
    in >> cond;
    in >> condChance;

    unsigned condValue = 0;
    for(unsigned i = 0; i < 7; i++){
        if(status[i] == cond){
            condValue = i;
        }
    }
    move = new StatusMove(name,type,strength,accuracy,isPhys,condValue,condChance);
    return move;
}

HealingMove* operator>>(istream& in, HealingMove*& move){
    string name;
    string type;
    unsigned strength;
    unsigned accuracy;
    bool isPhys;
    unsigned healPercentage;

    in >> name;
    in >> type;
    in >> strength;
    in >> accuracy;
    in >> isPhys;
    in >> healPercentage;

    move = new HealingMove(name,type,strength,accuracy,isPhys,healPercentage);
    return move;
}

BoostMove* operator>>(istream& in, BoostMove*& move){}