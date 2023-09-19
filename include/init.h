#ifndef INIT_H
#define INIT_H

#include "pokemon.h"
#include <cassert>
#include <iostream>
#include <fstream>

using std::cout, std::ifstream, std::string;

void initEffChart();
void initNatureChart();
void printTurnEnd(Pokemon&, Pokemon&);
void simulateBattle(Pokemon&, Pokemon&, unsigned);

#endif