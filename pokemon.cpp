#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <time.h>
#include <cassert>
#include <fstream>

using std::cout, std::vector, std::string, std::srand, std::ifstream, std::istream;

static double effChart[19][19];
const string status[7] = {"", "PAR", "PSN", "TOX", "BRN", "SLP", "FRZ"};

class Type{
    public:
    string typeName;
    unsigned typeValue;

    void typeNameToValue(string name){
        const string array[19] = {"Normal", "Fire", "Water", "Grass", "Electric", "Ice", "Fighting", "Poison", "Ground", "Flying", "Psychic", "Bug", "Rock", "Ghost", "Dragon", "Dark", "Steel", "Fairy", "none"};
        for(unsigned i=0; i < 19; i++){
            if(name == array[i]){
                typeValue = i;
                return;
            }
        }
        cout << "unknown Type: "<< name << "\n";
        assert(1==0);
    }

    Type(): typeName("none"){
        typeNameToValue("none");
    }
    Type(string n): typeName(n){
        typeNameToValue(n);
    }
};

class Move{
    public:
    string attackName;
    Type moveType;
    unsigned strength;
    unsigned accuracy;
    bool physicalMove;  //physical, special

    unsigned statusCondValue;
    unsigned chance;

    Move(): attackName("???"), moveType(Type()), strength(0), accuracy(0), physicalMove(true){}
    Move(string n, string t, unsigned d, unsigned acc, bool phys):
    attackName(n), moveType(Type(t)), strength(d), accuracy(acc), physicalMove(phys){
        statusCondValue = 0;
        chance = 0;
    }

    Move(string n, string t, unsigned d, unsigned acc, bool phys, unsigned sCV, unsigned c):
    attackName(n), moveType(Type(t)), strength(d), accuracy(acc), physicalMove(phys){
        statusCondValue = sCV;
        chance = c;
    }
    
    virtual unsigned moveEffect(){
        return 0;
    }
};

class StatusMove: public Move{
    public:
    StatusMove(): Move(){}
    StatusMove(string n, string t, unsigned s, unsigned acc, bool phys, unsigned scv, unsigned c): Move(n,t,s,acc,phys,scv,c){}

    unsigned moveEffect(){
        unsigned randVal = rand() % 100;
        if(randVal < chance){
            return statusCondValue;
        }
        return 0;
    }
};

class Pokemon{
    public:
    string name;
    Type typing[2];
    Move* moves[4];
    int stats[6]; //0=hp 1=attack 2=def 3=spatt 4=spdef 5=speed
    unsigned condValue;
    unsigned sleepCounter;
    unsigned toxCounter;
    unsigned sixPercentHP;

    void initMoves(vector<Move*> moveVector){
        assert(moveVector.size() != 0);
        assert(moveVector.size() < 5);
        for(unsigned i = 0; i < moveVector.size(); i++){
            moves[i] = moveVector[i];
        }
    }
    void initStats(vector<unsigned> statsVector){
        assert(statsVector.size() == 6);
        for(unsigned i = 0; i < statsVector.size(); i++){
            stats[i] = statsVector[i];
        }
    }
    void initNature(unsigned inc, unsigned dec){
        assert((inc != 0) && (dec != 0));
        assert((inc < 6) && (dec < 6));
        if(inc != dec){
            stats[inc] = stats[inc] + stats[inc]*0.1;
            stats[dec] = stats[dec] - stats[dec]*0.1;
        }
    }
    void initTyping(vector<Type*> typeVector){
        assert(typeVector.size() != 0);
        assert(typeVector.size() < 3);

        typing[0] = *typeVector[0];
        typing[1] = *typeVector[1];
    }
    //initializers

    Pokemon():name("noName"){
        for(unsigned i = 0; i < 4; i++){    //only 4 moves
            moves[i] = new Move();
        }
        for(unsigned i = 0; i < 6; i++){    //only 6 stats
            stats[i] = 1;
        }
    }
    Pokemon(string n, vector<Move*> moveVector, vector<unsigned> statsVector, vector<Type*> typeVector){
        name = n;
        initMoves(moveVector);
        initStats(statsVector);
        initTyping(typeVector);
        condValue = 0;
        sleepCounter = 0;
        toxCounter = 0;
        sixPercentHP = 0.06 * stats[0];
    }

    bool isAlive(){
        if(stats[0] <= 0) return false;
        return true;
    }

    bool canAttack(unsigned condValue, unsigned turnCounter){
        int randValue = rand() % 100;
        switch(condValue){
            case 1:
                if(randValue < 25){
                    cout << name << " is paralyzed! It's unable to attack!\n";
                    return false;
                }
                return true;
            case 5:
                if(turnCounter == 0){
                    cout << name << " is fast asleep...\n";
                    sleepCounter++;
                    return false;
                }
                else if(turnCounter == 3){
                    cout << name << " woke up!\n";
                    sleepCounter = 0;
                    condValue=0;
                    return true;
                }
                else{
                    if(randValue < 50){
                        cout << name << " woke up!\n";
                        sleepCounter = 0;
                        condValue=0;
                        return true;
                    }
                    else{
                        cout << name << " is fast asleep...\n";
                        sleepCounter++;
                        return false;
                    }
                }
            case 6:
                if(randValue < 10){
                    cout << name << " thawed!\n";
                    return true;
                }
                else{
                    cout << name << " is frozen solid.\n";
                    return false;
                }
            default:
                return true;
        }  
    }

    void takeStatusDamage(unsigned condValue, unsigned turnCounter){
        switch(condValue){
            case 2:
                cout << name << " took damage from poison!\n";
                stats[0] = stats[0] - (2*sixPercentHP);
                break;
            case 3:
                cout << name << " took damage from poison!\n";
                toxCounter++;
                stats[0] = stats[0] - ((turnCounter+1)*sixPercentHP);
                break;
            case 4:
                cout << name << " took damage from burn!\n";
                stats[0] -= sixPercentHP;
                break;
            default:
                break;
        }
    }

    void statusChangeMessage(unsigned condValue){
        switch(condValue){
            case 1:
            cout << name << " got paralyzed!\n";
            break;

            case 2:
            cout << name << " got poisoned!\n";
            break;
            
            case 3:
            cout << name << " got badly poisoned!\n";
            break;
            
            case 4:
            cout << name << " got burned!\n";
            break;
            
            case 5:
            cout << name << " fell asleep!\n";
            break;
            
            case 6:
            cout << name << " got frozen!\n";
            break;
        }
    }

    void attack(Pokemon& other, Move*& move){
        unsigned hit =  rand() % 100;
        unsigned crit = rand() % 100;

        unsigned tmpCondValue = other.condValue;

        if(isAlive() && other.isAlive() && canAttack(condValue, sleepCounter)){
            cout << name << " uses " << move->attackName << "\n";
            if(hit < move->accuracy){
                double damage;
                double typingMultiplier = 1;

                damage = move->strength;

                typingMultiplier = typingMultiplier*effChart[(move->moveType).typeValue][(other.typing[0]).typeValue]*effChart[(move->moveType).typeValue][(other.typing[1]).typeValue];


                if(move->physicalMove){
                    damage = damage*stats[1]/other.stats[2];
                }
                else{
                    damage = damage*stats[3]/other.stats[4];
                }
                if ((crit > 89) && (move->strength != 0)){
                    damage *= 1.5;
                    cout << "Critical hit!\n";
                }

                if(typingMultiplier > 1){
                    cout << "It's super effective!\n";
                }
                else if((typingMultiplier < 1) && (typingMultiplier != 0)){
                    cout << "It's not very effective...\n";
                }
                else if(typingMultiplier == 0){
                    cout << "It doesn't affect the opponent...\n";
                }

                damage *= typingMultiplier;

                if(((move->moveType).typeValue == typing[0].typeValue)||((move->moveType).typeValue == typing[1].typeValue)){
                    damage*=1.5;
                }
                //STAB Bonus

                other.stats[0] -= damage;
                if(other.condValue == 0){
                    other.condValue = move->moveEffect();
                }
                if((tmpCondValue != other.condValue) && (other.condValue != 0)){
                    other.statusChangeMessage(other.condValue);
                }
            }
            else{
                cout << name << "'s attack missed!\n";
            }
            if (!other.isAlive()){
                cout << other.name << " fainted!\n";
            }
            else{
                cout << other.name << " has " << other.stats[0] << "HP left.\n";
            }
        }
    }
};

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

Pokemon* operator>>(istream& in, Pokemon*& poke){
    string name;
    vector<Type*> typing;
    vector<unsigned> pokeStats;
    vector<Move*> moveset;
    
    Move* move;
    StatusMove* sMove;
    string command;
    string typeString;
    Type tmpType;
    int tmpValue;

    in >> command;
    assert(command == "Pokemon:");
    in >> name;
    for(unsigned i = 0; i < 2; i++){
        in >> typeString;
        typing.push_back(new Type(typeString));
    }

    in >> command;
    assert(command == "Stats:");
    for(unsigned i = 0; i < 6; i++){
        in >> tmpValue;
        pokeStats.push_back(tmpValue);
    }

    for(unsigned i = 0; i < 4; i++){
        in >> command;
        assert((command == "Move:") || (command == "StatusMove:"));
        if(command == "Move:"){
            in >> move;
            moveset.push_back(move);
        }
        else if(command == "StatusMove:"){
            in >> sMove;
            moveset.push_back(sMove);
        }
    }
    poke = new Pokemon(name,moveset,pokeStats,typing);
    return poke;
}

//void printTurnEnd(Pokemon& poke1, Pokemon& poke2){}

void simulateBattle(Pokemon& poke1, Pokemon& poke2, unsigned maxRounds){
    bool oneIsFaster = true;
    unsigned pickMove1, pickMove2;
    cout << "--------------------------------\n";
    for(unsigned i=0; i < maxRounds; i++){
        pickMove1 = rand() % 4;
        pickMove2 = rand() % 4;
        oneIsFaster = (poke1.stats[5] > poke2.stats[5]);
        if(oneIsFaster){
            poke1.attack(poke2, poke1.moves[pickMove1]);
            cout << "\n";
            poke2.attack(poke1, poke2.moves[pickMove2]);
            poke1.takeStatusDamage(poke1.condValue, poke1.toxCounter);
            poke2.takeStatusDamage(poke2.condValue, poke2.toxCounter);
            cout << "--------------------------------\n";
        }
        else{
            poke2.attack(poke1, poke2.moves[pickMove2]);
            cout << "\n";
            poke1.attack(poke2, poke1.moves[pickMove1]);
            poke2.takeStatusDamage(poke2.condValue, poke2.toxCounter);
            poke1.takeStatusDamage(poke1.condValue, poke1.toxCounter);
            cout << "--------------------------------\n";
        }
        if(!(poke1.isAlive() && poke2.isAlive())){
            break;
        }
    }
}

void initEffChart(){
    ifstream inputFile;
    inputFile.open("effChart.txt");
    for(unsigned i = 0; i < 18; i++){
        for(unsigned j = 0; j < 18; j++){
            inputFile >> effChart[i][j];
        }
        effChart[i][18] = 1;
        effChart[18][i] = 1;
    }
    effChart[18][18] = 1;
}

int main(int argc, char** argv){

    if(argc == 2){
        cout << "Starting simulation...\n";
    }
    srand(time(NULL));
    initEffChart();

    string filename = argv[1];
    //string filename = "input/poke2.txt";
    //string mode;
    ifstream inputFile;
    string command;
    Pokemon* pokemon1 = nullptr;
    Pokemon* pokemon2 = nullptr;
    unsigned rounds = 0;

    inputFile.open(filename);
    inputFile >> pokemon1;
    inputFile >> pokemon2;
    
    inputFile >> command;
    if(command == "Rounds:") 
    {
        inputFile >> rounds;
    }

    simulateBattle(*pokemon1,*pokemon2,rounds);
    inputFile.close();
    return 0;
}