#ifndef INIT_H
#define INIT_H

#include "pokemon.h"
#include <cassert>
#include <iostream>
#include <fstream>

using std::cout, std::ifstream, std::string;

static double effChart[19][19];
static string natureChart[5][5];
const string status[7] = {"", "PAR", "PSN", "TOX", "BRN", "SLP", "FRZ"};

void initEffChart();
void initNatureChart();
void printTurnEnd(Pokemon&, Pokemon&);
void simulateBattle(Pokemon&, Pokemon&, unsigned);

#endif