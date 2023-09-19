#include "pokemon.h"

void Pokemon::initMoveset(vector<Move*> moves){
        assert(moves.size() == 4);
        for(unsigned i = 0; i < moves.size(); i++){
            moveset[i] = moves[i];
        }
    }

void Pokemon::initStats(vector<unsigned> stats){
        assert(stats.size() == 6);
        for(unsigned i = 0; i < stats.size(); i++){
            baseStats[i] = stats[i];
            currentStats[i] = stats[i];
        }
    }

void Pokemon::initNature(string nameNature){
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

void Pokemon::initTyping(vector<Type*> types){
        assert(types.size() == 2);
        for(unsigned i = 0; i < types.size(); i++){
            typing[i] = *types[i];
        }
    }

Pokemon::Pokemon():name("noName"), conditionValue(0){
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

Pokemon::Pokemon(string n, string nat, vector<unsigned> stats, vector<Move*> moves, vector<Type*> types): name(n), conditionValue(0){
        initStats(stats);
        initMoveset(moves);
        initTyping(types);
        initNature(nat);
        turnCounters[SLEEP] = 0; turnCounters[TOXIC] = 0; 
}

bool Pokemon::isAlive(){
        if(currentStats[0] <= 0){
            currentStats[0] = 0;
            return false;
        }
        return true;
    }

bool Pokemon::canAttack(unsigned condValue){
        int randValue = rand() % 100;
        switch(condValue){
            case 1:
                if(randValue < 25){
                    cout << name << " is paralyzed! It's unable to attack!\n";
                    return false;
                }
                return true;
            case 5:
                if(turnCounters[SLEEP] == 0){
                    cout << name << " is fast asleep...\n";
                    turnCounters[SLEEP]++;
                    return false;
                }
                else if(turnCounters[SLEEP] == 3){
                    cout << name << " woke up!\n";
                    turnCounters[SLEEP] = 0;
                    condValue=0;
                    return true;
                }
                else{
                    if(randValue < 50){
                        cout << name << " woke up!\n";
                        turnCounters[SLEEP] = 0;
                        condValue=0;
                        return true;
                    }
                    else{
                        cout << name << " is fast asleep...\n";
                        turnCounters[SLEEP]++;
                        return false;
                    }
                }
            case 6:
                if(randValue < 20){
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

void Pokemon::takeStatusDamage(unsigned condValue){
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

        if(!isAlive()){
            cout << name << " fainted!\n";
            return;
        }
    }
    
void Pokemon::statusChangeMessage(unsigned condValue){
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

void Pokemon::statusStatChange(unsigned condValue){
        switch(condValue){
            case 1:
                currentStats[5] *= 0.25;
                break;
            case 4:
                currentStats[1] *= 0.5;
                break;
            default:
                return;
        }
    }

unsigned Pokemon::damageCalc(Pokemon& other, Move*& move){
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

        bool damageIsNotZero = (damage != 0);
        if((crit > 89) && damageIsNotZero){
            damage*=1.5;
            cout << "Critical hit!\n";
        }
        if((typingMultiplier > 1) && damageIsNotZero){
            cout << "It's super effective!\n";
        }
        else if(typingMultiplier == 0){
            cout << "It doesn't affect the opponent...\n";
        }
        else if((typingMultiplier < 1) && damageIsNotZero){
            cout << "It's not very effective...\n";
        }

        if(damage > other.currentStats[0]){
            damage = other.currentStats[0];
        }
        return damage;
    }

void Pokemon::addHP(unsigned HP){
    currentStats[0] += HP;
    if(currentStats[0] > baseStats[0]){
        currentStats[0] = baseStats[0];
    }
}

void Pokemon::attack(Pokemon& other, Move*& move){
        unsigned hit = rand() % 100;
        unsigned tmpCondValue = other.conditionValue;
        unsigned healing = 0;

        if(isAlive() && other.isAlive() && canAttack(conditionValue)){
            cout << name << " uses " << move->attackName << "\n";
            if(hit < move->accuracy){
                other.currentStats[0] -= damageCalc(other,move);
                healing = move->moveHealing(damageCalc(other,move));
                addHP(healing);

                if(other.isAlive() && (other.conditionValue == 0)){
                    other.conditionValue = move->moveEffect();
                    if(other.conditionValue != tmpCondValue){
                        other.statusChangeMessage(other.conditionValue);
                        other.statusStatChange(other.conditionValue);
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

Pokemon* operator>>(istream& in, Pokemon*& poke){
    string name;
    string nature;
    vector<Type*> typing;
    vector<unsigned> pokeStats;
    vector<Move*> moveset;

    Move* move;
    StatusMove* sMove;
    HealingMove* hMove;
    BoostMove* bMove;
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
        if(command == "Move:"){
            in >> move;
            moveset.push_back(move);
        }
        else if(command == "StatusMove:"){
            in >> sMove;
            moveset.push_back(sMove);
        }
        else if(command == "HealingMove:"){
            in >> hMove;
            moveset.push_back(hMove);
        }
        else{
            cout << "Unknown command: " << command << "\n";
            bool moveUnknown = false;
            assert(moveUnknown);
        }
    }
    poke = new Pokemon(name,nature,pokeStats,moveset,typing);
    return poke;
}