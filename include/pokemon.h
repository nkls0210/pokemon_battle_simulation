#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "moves.h"
#include "type.h"

using std::cout, std::vector, std::string, std::istream;

#define SLEEP 0
#define TOXIC 1

inline double effChart[19][19];
inline string natureChart[5][5];

class Pokemon{
    public:
    string name;
    string nature;
    int baseStats[6];
    int currentStats[6];
    int buffs[5]; // 0 = atk.... 4 = speed
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

    void addHP(unsigned);

    void updateStats(vector<int>);

    void attack(Pokemon&, Move*&);
};

Pokemon* operator>>(istream&, Pokemon*&);

#endif