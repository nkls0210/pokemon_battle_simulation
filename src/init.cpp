#include "init.h"

void initEffChart(){
    cout << "Reading Efficiency Chart...";
    ifstream inputFile;
    inputFile.open("init/effChart.txt");
    if(!inputFile.is_open()){
        cout << "\n Error: effChart.txt not found\n";
        assert(inputFile.is_open());
    }
    for(unsigned i = 0; i < 18; i++){
        for(unsigned j = 0; j < 18; j++){
            inputFile >> effChart[i][j];
        }
        effChart[i][18] = 1;
        effChart[18][i] = 1;
    }
    effChart[18][18] = 1;
    cout << "Done!\n";
    inputFile.close();
}

void initNatureChart(){
    cout << "Reading Nature Chart...";
    ifstream inputFile;
    inputFile.open("init/natureChart.txt");
    if(!inputFile.is_open()){
        cout << "\n Error: natureChart.txt not found\n";
        assert(inputFile.is_open());
    }
    for(unsigned i = 0; i < 5; i++){
        for(unsigned j = 0; j < 5; j++){
            inputFile >> natureChart[i][j];
        }
    }
    cout << "Done!\n";
    inputFile.close();
}

void printTurnEnd(Pokemon& poke1, Pokemon& poke2){
    cout << "\n";
    cout << poke1.name << " HP[" << poke1.currentStats[0] << "/" << poke1.baseStats[0] << "]";
    if(poke1.conditionValue != 0){
        cout << "("<< status[poke1.conditionValue] << ")    ";
    }
    else{
        cout << "         ";
    }
    cout << poke2.name << " HP[" << poke2.currentStats[0] << "/" << poke2.baseStats[0] << "]";
    if(poke2.conditionValue != 0){
        cout << "("<< status[poke2.conditionValue] << ")\n";
    }
    else{
        cout << "\n";
    }   
}

void printStats(Pokemon& poke){
    cout << poke.name << "'s Stats:\n";
    for(unsigned i = 0; i < 5; i++){
        cout << stat[i] << ": " << poke.currentStats[i+1] << "(" << poke.buffs[i] << ")\n";
    }
}

void simulateBattle(Pokemon& poke1, Pokemon& poke2, unsigned maxRounds){
    bool oneIsFaster = true;
    unsigned pickMove1, pickMove2;
    cout << "--------------------------------\n";
    for(unsigned i=0; i < maxRounds; i++){
        pickMove1 = rand() % 4;
        pickMove2 = rand() % 4;
        oneIsFaster = (poke1.currentStats[5] > poke2.currentStats[5]);
        if(oneIsFaster){
            poke1.attack(poke2, poke1.moveset[pickMove1]);
            cout << "\n";
            poke2.attack(poke1, poke2.moveset[pickMove2]);
            poke1.takeStatusDamage(poke1.conditionValue);
            poke2.takeStatusDamage(poke2.conditionValue);
        }
        else{
            poke2.attack(poke1, poke2.moveset[pickMove2]);
            cout << "\n";
            poke1.attack(poke2, poke1.moveset[pickMove1]);
            poke2.takeStatusDamage(poke2.conditionValue);
            poke1.takeStatusDamage(poke1.conditionValue);
        }
        printTurnEnd(poke1,poke2);
        cout << "--------------------------------\n";
        if(!(poke1.isAlive() && poke2.isAlive())){
            break;
        }
    }
}
