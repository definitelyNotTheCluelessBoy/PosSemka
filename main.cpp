#include <thread>
#include <queue>
#include "Socket.h"
#include "Simulation.h"



void produce(Simulation& data) {
    data.simulate();
}

void consume(Simulation& data) {
    data.listenInput();
}

int main() {
    auto* socket = new Socket(17319, "frios2.fri.uniza.sk");

    Simulation simulation(3,0.4,0.2,0.2,0.2,socket);
    thread simulationThread(produce, ref(simulation));
    thread listeningThread(consume, ref(simulation));
    simulationThread.join();
    listeningThread.join();
    delete socket;
    return 0;
}
