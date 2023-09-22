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
vector<int>  Move::moveBuff(){
    vector<int> tmp;
    for(unsigned i = 0; i < 5; i++){
        tmp.push_back(0);
    }
    return tmp;
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
unsigned StatusMove::moveHealing(unsigned buf){
    buf = 0;
    return buf;
};
vector<int>  StatusMove::moveBuff(){
    vector<int> tmp;
    for(unsigned i = 0; i < 5; i++){
        tmp.push_back(0);
    }
    return tmp;
}


HealingMove::HealingMove(): Move(), healingPrecent(0){}
HealingMove::HealingMove(string n, string t, unsigned s, unsigned acc, bool phys, unsigned heal): Move(n,t,s,acc,phys), healingPrecent(heal){};
unsigned HealingMove::moveEffect(){
    return 0;
}
unsigned HealingMove::moveHealing(unsigned damage){
    unsigned healValue = damage*healingPrecent/100;
    return healValue;
}
vector<int>  HealingMove::moveBuff(){
    vector<int> tmp;
    for(unsigned i = 0; i < 5; i++){
        tmp.push_back(0);
    }
    return tmp;
}

BuffMove::BuffMove():Move(){
    buffChance = 0;
    for(unsigned i = 0; i < 5; i++){
        statBuffs[i] = 0;
    }
}
BuffMove::BuffMove(string n, string t, unsigned s, unsigned acc, bool phys, vector<int> buffVector, unsigned chance):
Move(n,t,s,acc,phys), buffChance(chance){
    assert(buffVector.size() == 5);
    for(unsigned i = 0; i < buffVector.size(); i++){
        statBuffs[i] = buffVector[i];
    }
};
unsigned BuffMove::moveEffect(){
    return 0;
};
unsigned BuffMove::moveHealing(unsigned buf){
    buf = 0;
    return buf;
};
vector<int>  BuffMove::moveBuff(){
    unsigned randVal = rand() % 100;
    vector<int> ret;
    if(randVal < buffChance){
        for(unsigned i = 0; i < 5; i++){
            ret.push_back(statBuffs[i]);
        }
        return ret;
    }
    else{
        for(unsigned i = 0; i < 5; i++){
            ret.push_back(0);
        }
        return ret;
    }
}

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

BuffMove* operator>>(istream& in, BuffMove*& move){
    string name;
    string type;
    unsigned strength;
    unsigned accuracy;
    bool isPhys;
    vector<int> buffs;
    unsigned chance;
    int tmpChanges;

    in >> name;
    in >> type;
    in >> strength;
    in >> accuracy;
    in >> isPhys;

    for(unsigned i = 0; i < 5; i++){
        in>>tmpChanges;
        buffs.push_back(tmpChanges);
    }

    in >> chance;

    move = new BuffMove(name,type,strength,accuracy,isPhys,buffs,chance);
    return move;
}