#ifndef MOVES_H
#define MOVES_H

#include <string>
#include <iostream>
#include <cassert>
#include "type.h"

using std::istream, std::string;

const string status[7] = {"", "PAR", "PSN", "TOX", "BRN", "SLP", "FRZ"};

class Move{
    public:
    string attackName;
    Type moveType;
    unsigned strength;
    unsigned accuracy;
    bool physicalMove;  //physical, special

    Move();
    Move(string n, string t, unsigned d, unsigned acc, bool phys);
    
    virtual unsigned moveEffect();
};

class StatusMove: public Move{
    public:
    unsigned statusCondValue;
    unsigned chance;
    StatusMove();
    StatusMove(string n, string t, unsigned s, unsigned acc, bool phys, unsigned scv, unsigned c);

    unsigned moveEffect();
};

class HealingMove: public Move{
    public:
    unsigned healingPrecent;
};

class BoostMove: public Move{
    public:
    int statBoosts[5];
};

Move* operator>>(istream&, Move*&);
StatusMove* operator>>(istream&, StatusMove*&);

#endif