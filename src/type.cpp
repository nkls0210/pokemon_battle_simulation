#include "type.h"


void Type::typeNameToValue(string name){
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

Type::Type(): typeName("none"){
        typeNameToValue("none");
    }

Type::Type(string n): typeName(n){
        typeNameToValue(n);
    }