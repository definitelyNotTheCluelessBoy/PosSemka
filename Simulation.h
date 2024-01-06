//
// Created by Matúš on 03/01/2024.
//

#ifndef SEMESTRALKACLIENT_SIMULATION_H
#define SEMESTRALKACLIENT_SIMULATION_H

#include "Cell.h"
#include <vector>
#include <random>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <sstream>
#include <iostream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;



class Simulation {
private:
    mutex mutVar;
    bool hold;

    vector<vector<Cell>> playField;

    int playFieldSize;
    string wind;
    int windCountDown;
    bool run;

    bool setCellAflame(int Y, int X);
    void setCellsAflameManually(int numberOfCells);
    bool turnCellIntoPlain(int Y, int X);
    bool turnCellIntoForest(int Y, int X);
    void generateWind();
    void printOut();
    string playingFieldToString();
    static string read(tcp::socket & socket);
    static void write(string stringtowrite, tcp::socket & socket);
    void replace(string message);
    static vector<string> split(string stringToParse, char delimeter);

public:
    Simulation(int size, double probabilityForTrees, double probabilityForWater, double probabilityPlains, double probabilityForRocks);
    ~Simulation();
    void simulate();
    void listenInput();
};


#endif //SEMESTRALKACLIENT_SIMULATION_H
