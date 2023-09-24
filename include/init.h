#ifndef INIT_H
#define INIT_H

#include "pokemon.h"
#include <cassert>
#include <iostream>
#include <fstream>

using std::cout, std::ifstream, std::string, std::cin;

void initEffChart();
void initNatureChart();
void printPokemonHP(Pokemon&, Pokemon&);
void printStats(Pokemon&);
void printStatsShort(Pokemon&);
void printMenuVSCPU(Pokemon&, Pokemon&);
void computerBattle(Pokemon&, Pokemon&, unsigned);
void humanVsComputer(Pokemon&, Pokemon&, unsigned, string);

#endif