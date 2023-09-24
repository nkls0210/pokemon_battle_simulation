#include "pokemon.h"

void Pokemon::initMoveset(vector<Move*> moves){
        assert(moves.size() < 5);
        for(unsigned i = 0; i < moves.size(); i++){
            moveset.push_back(moves[i]);
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
                    firstStats[i+1] *= 1.1;
                    firstStats[j+1] *= 0.9;
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

void Pokemon::initEVs(vector<unsigned> evVec){
    assert(evVec.size() == 6);
    for(unsigned i = 0; i < 6; i++){
        assert(evVec[i] < 253);
        evs[i] = evVec[i];
    }
}

void Pokemon::initIVs(vector<unsigned> ivVec){
    assert(ivVec.size() == 6);
    for(unsigned i = 0; i < 6; i++){
        assert(ivVec[i] < 32);
        ivs[i] = ivVec[i];
    }
}

void Pokemon::calcFirstStats(){
    unsigned tmpCalc;
    unsigned evCalc;

    evCalc = evs[0]/4;
    tmpCalc = (2*baseStats[0] + ivs[0] + evCalc)*level/100;
    firstStats[0] = tmpCalc+level+10;

    for(unsigned i = 1; i < 6; i++){
        evCalc = evs[i]/4;
        tmpCalc = (2*baseStats[i] + ivs[i] + evCalc)*level/100;
        firstStats[i] = tmpCalc+5;
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

        for(unsigned i = 0; i < 5; i++){
            buffs[i] = 0;
        }
    }

Pokemon::Pokemon(string n, string nat, unsigned lvl, vector<unsigned> stats, vector<unsigned> ev, vector<unsigned> iv, vector<Move*> moves, vector<Type*> types): name(n),level(lvl), conditionValue(0){
        initStats(stats);
        initMoveset(moves);
        initTyping(types);
        initEVs(ev);
        initIVs(iv);
        turnCounters[SLEEP] = 0; turnCounters[TOXIC] = 0; 
        for(unsigned i = 0; i < 5; i++){
            buffs[i] = 0;
        }
        calcFirstStats();
        initNature(nat);
        for(unsigned i = 0; i < 6; i++){
            currentStats[i] = firstStats[i];
        }
}

bool Pokemon::isAlive(){
        if(currentStats[0] <= 0){
            currentStats[0] = 0;
            return false;
        }
        return true;
    }

bool Pokemon::canAttack(){
        int randValue = rand() % 100;
        switch(conditionValue){
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
                    conditionValue=0;
                    return true;
                }
                else{
                    if(randValue < 50){
                        cout << name << " woke up!\n";
                        turnCounters[SLEEP] = 0;
                        conditionValue=0;
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
                currentStats[0] = currentStats[0] - 0.12*firstStats[0];
                break;
            case 3:
                cout << name << " took damage from poison!\n";
                turnCounters[TOXIC]++;
                currentStats[0] = currentStats[0] - (turnCounters[TOXIC]*0.06)*firstStats[0];
                break;
            case 4:
                cout << name << " took damage from burn!\n";
                currentStats[0] = currentStats[0] - 0.06*firstStats[0];
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
        int damage;
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
    if(currentStats[0] > firstStats[0]){
        currentStats[0] = firstStats[0];
    }
}

void Pokemon::convertBuffs(){
    for(unsigned i=0; i < 5; i++){
        if(buffs[i] > 0){
            currentStats[i+1] = firstStats[i+1]*(buffs[i]+2)/2;
        }
        else if(buffs[i] < 0){
            currentStats[i+1] = firstStats[i+1]*2/((buffs[i]*(-1))+2);
        }
    }
}

void Pokemon::updateBuffs(vector<int> statBuffs){
    for(unsigned i = 0; i < 5; i++){
        if(statBuffs[i] > 0){
            if(buffs[i] != 6){
                cout << name << "'s " << stat[i] << " rose";
                if((statBuffs[i] >= 2) &&(buffs[i] < 5)){
                    cout << " sharply";
                }
                cout << "!\n";
            }
            else{
                cout << name << "'s " << stat[i] << " can't go any higher!\n";
            }
        }
        else if(statBuffs[i] < 0){
            if(buffs[i] != -6){
                cout << name << "'s " << stat[i] << " fell";
                if((statBuffs[i] <= -2) &&(buffs[i] > -5)){
                    cout << " sharply";
                }
                cout << "!\n";
            }
            else{
                cout << name << "'s " << stat[i] << " can't go any lower!\n";
            }
        }
        buffs[i] += statBuffs[i];
        if(buffs[i] > 6){
            buffs[i] = 6;
        }
        else if(buffs[i] < -6){
            buffs[i] = -6;
        }
    }
}

void Pokemon::attack(Pokemon& other, Move*& move){
        unsigned hit = rand() % 100;
        unsigned tmpCondValue = other.conditionValue;
        unsigned healing = 0;
        unsigned damage;

        if(isAlive() && canAttack() && other.isAlive()){
            cout << name << " uses " << move->attackName << "\n";
            if(hit < move->accuracy){
                damage = damageCalc(other,move);
                other.currentStats[0] -= damage;
                healing = move->moveHealing(damage);
                if(healing != 0){
                    cout << name << " healed "<< healing << " HP\n";
                    addHP(healing);
                }

                updateBuffs(move->moveBuff());
                convertBuffs();
                statusStatChange(conditionValue);

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
    ifstream pokedex;
    pokedex.open("init/gen_1.txt");
    assert(pokedex.is_open());

    string tmpString;
    string type;
    string command;
    string name;
    string nature;
    unsigned level;
    unsigned statNum;
    vector<unsigned> baseStats;
    vector<unsigned> EVs;
    vector<unsigned> IVs;
    vector<Move*> moves;
    vector<Type*> types;

    in >> name;

    while(pokedex.peek() != EOF){
        pokedex >> tmpString;
        if(tmpString == name){
            pokedex >> type;
            types.push_back(new Type(type));
            pokedex >> type;
            types.push_back(new Type(type));

            for(unsigned i = 0; i < 6; i++){
                pokedex >> statNum;
                baseStats.push_back(statNum);
            }
            break;
        }
    }
    pokedex.close();

    in >> command;
    assert(command == "Level:");
    in >> level;

    in >> command;
    assert(command == "Nature:");
    in >> nature;

    in >> command;
    assert(command == "EV:");
    for(unsigned i = 0; i < 6; i++){
        in >> statNum;
        assert(statNum < 253);
        EVs.push_back(statNum);
    }

    in >> command;
    assert(command == "IV:");
    for(unsigned i = 0; i < 6; i++){
        in >> statNum;
        assert(statNum < 32);
        IVs.push_back(statNum);
    }
    
    for(unsigned i = 0; i < 4; i++){
        in >> command;
        if(command == "Move:"){
            Move* tmp;
            in >> tmp;
            moves.push_back(tmp); 
        }
        else if(command == "StatusMove:"){
            StatusMove* tmp;
            in >> tmp;
            moves.push_back(tmp); 
        }
        else if(command == "HealingMove:"){
            HealingMove* tmp;
            in >> tmp;
            moves.push_back(tmp); 
        }
        else if(command == "BuffMove:"){
            BuffMove* tmp;
            in >> tmp;
            moves.push_back(tmp); 
        }
    }

    poke = new Pokemon(name,nature,level,baseStats,EVs,IVs,moves,types);
    return poke;
}