#ifndef MOVES_H
#define MOVES_H

#include <string>
#include <vector>
#include <iostream>
#include <cassert>
#include "type.h"

using std::istream, std::string, std::vector;

const string status[7] = {"", "PAR", "PSN", "TOX", "BRN", "SLP", "FRZ"};
const string statNames[6] = {"", "ATK", "DEF", "SPA", "SPD", "SPE"};

class Move{
    public:
    string attackName;
    Type moveType;
    unsigned strength;
    unsigned accuracy;
    bool physicalMove;  //physical, special

    Move();
    Move(string, string, unsigned, unsigned, bool);
    
    virtual unsigned moveEffect();
    virtual unsigned moveHealing(unsigned);
    
};

class StatusMove: public Move{
    public:
    unsigned statusCondValue;
    unsigned chance;

    StatusMove();
    StatusMove(string, string, unsigned, unsigned, bool, unsigned, unsigned);

    unsigned moveEffect();
    unsigned moveHealing(unsigned);
};

class HealingMove: public Move{
    public:
    unsigned healingPrecent;
    
    HealingMove();
    HealingMove(string, string, unsigned, unsigned, bool, unsigned);

    unsigned moveEffect();
    unsigned moveHealing(unsigned);
};

class BuffMove: public Move{
    public:
    int statBuffs[5];
    unsigned buffChance;

    BuffMove();
    BuffMove(string, string, unsigned, unsigned, bool, vector<int>, unsigned);
    
    unsigned moveEffect();
    unsigned moveHealing(unsigned);
};

Move* operator>>(istream&, Move*&);
StatusMove* operator>>(istream&, StatusMove*&);
HealingMove* operator>>(istream&, HealingMove*&);
BuffMove* operator>>(istream&, BuffMove*&);

#endif