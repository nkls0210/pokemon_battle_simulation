#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <time.h>
#include <cassert>
#include <fstream>

using std::cout, std::vector, std::string, std::srand, std::ifstream, std::istream;

static double effChart[19][19];
static string natureChart[5][5];
const string status[7] = {"", "PAR", "PSN", "TOX", "BRN", "SLP", "FRZ"};
#define SLEEP 0
#define TOXIC 1

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
    string nature;
    unsigned baseStats[6]; // HP/Atk/Def/SpA/SpD/Spe
    int currentStats[6];
    Move* moveset[4];
    Type typing[2];
    unsigned conditionValue;
    unsigned turnCounters[2]; // 0=sleep timer, 1=toxic timer

    void initMoveset(vector<Move*> moves){
        assert(moves.size() == 4);
        for(unsigned i = 0; i < moves.size(); i++){
            moveset[i] = moves[i];
        }
    }

    void initStats(vector<unsigned> stats){
        assert(stats.size() == 6);
        for(unsigned i = 0; i < stats.size(); i++){
            baseStats[i] = stats[i];
            currentStats[i] = stats[i];
        }
    }

    void initTyping(vector<Type*> types){
        assert(types.size() == 2);
        for(unsigned i = 0; i < types.size(); i++){
            typing[i] = *types[i];
        }
    }

    void initNature(string nameNature){
        nature = nameNature;
        for(unsigned i = 0; i < 5; i++){
            for(unsigned j = 0; j < 5; j++){
                if(natureChart[i][j] == nameNature){
                    currentStats[i+1] = currentStats[i+1] + baseStats[i+1]*0.1;
                    currentStats[j+1] = currentStats[j+1] - baseStats[j+1]*0.1;
                    break;
                }
            }
        }
    }

    Pokemon():name("noName"), conditionValue(0){
        for(unsigned i = 0; i < 6; i++){
            baseStats[i] = 1;
            currentStats[i] = baseStats[i];
        }

        for(unsigned i = 0; i < 4; i++){
            moveset[i] = new Move();
        }

        for(unsigned i = 0; i < 2; i++){
            typing[i] = Type("none");
            turnCounters[i] = 0;
        }
    }

    Pokemon(string n, string nat, vector<unsigned> stats, vector<Move*> moves, vector<Type*> types): name(n), conditionValue(0){
        initStats(stats);
        initMoveset(moves);
        initTyping(types);
        initNature(nat);
        turnCounters[SLEEP] = 0; turnCounters[TOXIC] = 0; 
    }
    
    bool isAlive(){
        if(currentStats[0] <= 0){
            currentStats[0] = 0;
            return false;
        }
        return true;
    }

    bool canAttack(unsigned condValue, unsigned sleepCounter){
        int randValue = rand() % 100;
        switch(condValue){
            case 1:
                if(randValue < 25){
                    cout << name << " is paralyzed! It's unable to attack!\n";
                    return false;
                }
                return true;
            case 5:
                if(sleepCounter == 0){
                    cout << name << " is fast asleep...\n";
                    turnCounters[SLEEP]++;
                    return false;
                }
                else if(sleepCounter == 3){
                    cout << name << " woke up!\n";
                    turnCounters[SLEEP] = 0;
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

    void takeStatusDamage(unsigned condValue){
        if(!isAlive()){
            return;
        }
        
        switch(condValue){
            case 2:
                cout << name << " took damage from poison!\n";
                currentStats[0] = currentStats[0] - 0.12*baseStats[0];
                break;
            case 3:
                cout << name << " took damage from poison!\n";
                turnCounters[TOXIC]++;
                currentStats[0] = currentStats[0] - (turnCounters[TOXIC]*0.06)*baseStats[0];
                break;
            case 4:
                cout << name << " took damage from burn!\n";
                currentStats[0] = currentStats[0] - 0.06*baseStats[0];
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

    unsigned damageCalc(Pokemon& other, Move*& move){
        unsigned crit = rand() % 100;
        unsigned damage;
        double typingMultiplier = 1;

        damage = move->strength;
        typingMultiplier *= effChart[(move->moveType).typeValue][(other.typing[0]).typeValue];
        typingMultiplier *= effChart[(move->moveType).typeValue][(other.typing[1]).typeValue];
        damage*=typingMultiplier;

        if(move->physicalMove){
            damage = damage*currentStats[1]/other.currentStats[2];
        }
        else{
            damage = damage*currentStats[3]/other.currentStats[4];
        }

        if((typing[0].typeValue == (move->moveType).typeValue) ||(typing[1].typeValue == (move->moveType).typeValue)){
            damage*= 1.5;
        }
        // STAB Bonus

        if(crit > 89){
            damage*=1.5;
            cout << "Critical hit!\n";
        }
        if(typingMultiplier > 1){
            cout << "It's super effective!\n";
        }
        else if(typingMultiplier == 0){
            cout << "It doesn't affect the opponent...\n";
        }
        else if(typingMultiplier < 1){
            cout << "It's not very effective...\n";
        }
        return damage;
    }

    void attack(Pokemon& other, Move*& move){
        unsigned hit = rand() % 100;
        unsigned tmpCondValue = other.conditionValue;

        if(isAlive() && other.isAlive() && canAttack(conditionValue, turnCounters[SLEEP])){
            cout << name << " uses " << move->attackName << "\n";
            if(hit < move->accuracy){
                other.currentStats[0] -= damageCalc(other,move);

                if(other.isAlive() && (other.conditionValue == 0)){
                    other.conditionValue = move->moveEffect();
                    if(other.conditionValue != tmpCondValue){
                        other.statusChangeMessage(other.conditionValue);
                    }
                }
                else if(!other.isAlive()){
                    cout << other.name << " fainted!\n";
                }
            }
            else{
                cout << name << "'s attack missed!\n";
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
    string nature;
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
    assert(command == "Nature:");
    in >> nature;

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
    poke = new Pokemon(name,nature,pokeStats,moveset,typing);
    return poke;
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
}

int main(int argc, char** argv){

    if(argc == 2){
        cout << "Starting simulation...\n";
        cout << "Input path: " << argv[1] <<"\n";
    }

    srand(time(NULL));
    initEffChart();
    initNatureChart();

    string filename = argv[1];
    //string filename = "input/pokemon.txt";
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