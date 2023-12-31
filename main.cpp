#include "init.h"
#include "pokemon.h"
#include "type.h"
#include "moves.h"


const string modes[] = {"simul", "battlecpu", "teambattle"};

int main(int argc, char** argv){

    if(argc == 4){
        cout << "Starting simulation...\n";
        cout << "Input path: " << argv[1] <<"\n";
        cout << "Mode: " << argv[2] << "\n";
        cout << "CPU Mode: " << argv[3] << "\n";
    }
    else{
        bool argumentError = false;
        assert(argumentError);
    }

    srand(time(NULL));
    initEffChart();
    initNatureChart();
    
    string filename = argv[1];
    //string filename = "input/pokemon.txt";
    string mode = argv[2];
    string cpuMode = argv[3];
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

    if(mode == modes[0]){
        computerBattle(*pokemon1,*pokemon2,rounds);
    }
    else if(mode == modes[1]){
        humanVsComputer(*pokemon1, *pokemon2, rounds, cpuMode);
    }
    else if(mode == modes[2]){
        cout << "Will be added in the future...";
    }

    inputFile.close();
    return 0;
}