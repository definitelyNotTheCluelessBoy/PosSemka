//
// Created by Matúš on 03/01/2024.
//

#ifndef SEMESTRALKACLIENT_SIMULATION_H
#define SEMESTRALKACLIENT_SIMULATION_H

#include "Cell.h"
#include "Socket.h"
#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <windows.h>
#include <mutex>
#include <condition_variable>
#include <sstream>

using namespace std;


class Simulation {
private:
    Socket* socket;
    mutex mutVar;
    bool hold;

    vector<vector<Cell*>> playField;

    int playFieldSize;
    char wind;
    int windCountDown;
    bool run;

    bool setCellAflame(int Y, int X);
    void setCellsAflameManually(int numberOfCells);
    bool turnCellIntoPlain(int Y, int X);
    bool turnCellIntoForest(int Y, int X);
    void generateWind();
    void printOut();
    string playingFieldToString();

public:
    Simulation(int size, double probabilityForTrees, double probabilityForWater, double probabilityPlains, double probabilityForRocks, Socket* socket);
    ~Simulation();
    void simulate();
    void listenInput();
};


#endif //SEMESTRALKACLIENT_SIMULATION_H
