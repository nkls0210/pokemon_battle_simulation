#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include "moves.h"
#include "type.h"

using std::cout, std::vector, std::ifstream, std::string, std::istream;

#define SLEEP 0
#define TOXIC 1

inline double effChart[19][19];
inline string natureChart[5][5];

class Pokemon{
    public:
    string name;
    string nature;
    unsigned level;
    int baseStats[6];
    unsigned evs[6];
    unsigned ivs[6];
    int firstStats[6];
    int currentStats[6];
    int buffs[5]; // 0 = atk.... 4 = speed
    vector<Move*> moveset;
    Type typing[2];
    unsigned conditionValue;
    unsigned turnCounters[2];

    void initMoveset(vector<Move*>);
    void initStats(vector<unsigned>);
    void initNature(string);
    void initTyping(vector<Type*>);
    void initEVs(vector<unsigned>);
    void initIVs(vector<unsigned>);
    void calcFirstStats();
    //initializers

    Pokemon();
    Pokemon(string, string, unsigned, vector<unsigned>, vector<unsigned>, vector<unsigned>, vector<Move*>, vector<Type*>);

    bool isAlive();

    bool canAttack();

    void takeStatusDamage(unsigned);

    void statusChangeMessage(unsigned);
    
    void statusStatChange(unsigned);

    unsigned damageCalc(Pokemon&, Move*&);

    void addHP(unsigned);

    void convertBuffs();

    void updateBuffs(vector<int>);

    void attack(Pokemon&, Move*&);
};


Pokemon* operator>>(istream&, Pokemon*&);

#endif