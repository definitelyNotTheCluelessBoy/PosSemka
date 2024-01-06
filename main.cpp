#include <thread>
#include <queue>
#include "Simulation.h"



void produce(Simulation& data) {
    data.simulate();
}

void consume(Simulation& data) {
    data.listenInput();
}

int main() {

    Simulation simulation(3,0.4,0.2,0.2,0.2);
    thread simulationThread(produce, ref(simulation));
    thread listeningThread(consume, ref(simulation));
    simulationThread.join();
    listeningThread.join();
    return 0;
}
