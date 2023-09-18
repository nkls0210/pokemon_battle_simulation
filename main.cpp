#include "pokemon.h"
#include "init.h"
#include "type.h"
#include "moves.h"


int main(int argc, char** argv){

    if(argc == 2){
        cout << "Starting simulation...\n";
        cout << "Input path: " << argv[1] <<"\n";
    }

    srand(time(NULL));
    initEffChart();
    initNatureChart();

    //string filename = argv[1];
    string filename = "input/pokemon.txt";
    //string mode = argv[2];
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