#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "moves.h"
#include "init.h"
#include "type.h"

using std::cout, std::vector, std::string, std::istream;

#define SLEEP 0
#define TOXIC 1

class Pokemon{
    public:
    string name;
    string nature;
    unsigned baseStats[6];
    int currentStats[6];
    Move* moveset[4];
    Type typing[2];
    unsigned conditionValue;
    unsigned turnCounters[2];

    void initMoveset(vector<Move*>);
    void initStats(vector<unsigned>);
    void initNature(string);
    void initTyping(vector<Type*>);
    //initializers

    Pokemon();
    Pokemon(string, string, vector<unsigned>, vector<Move*>, vector<Type*>);

    bool isAlive();

    bool canAttack(unsigned);

    void takeStatusDamage(unsigned);

    void statusChangeMessage(unsigned);
    
    void statusStatChange(unsigned);

    unsigned damageCalc(Pokemon&, Move*&);

    void attack(Pokemon&, Move*&);
};

Pokemon* operator>>(istream&, Pokemon*&);

#endif