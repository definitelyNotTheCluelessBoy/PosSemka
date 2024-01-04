#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <random>
#include <string>
#include "my_socket.h"
#include "Simulation.h"



void produce(Simulation& data) {
    data.simulate();
}

void consume(Simulation& data) {
    data.listenInput();
}

int main() {

    Simulation simulation(9,0.4,0.2,0.2,0.2);
    thread simulationThread(produce, ref(simulation));
    thread listeningThread(consume, ref(simulation));
    simulationThread.join();
    listeningThread.join();
    return 0;
}
