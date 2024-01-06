executeable: Cell.o Simulation.o main.o
	g++ main.o Simulation.o Cell.o -o executeable
main.o: main.cpp
	g++ -c main.cpp
Cell.o: Cell.cpp Cell.h
	g++ -c Cell.cpp
Simulation.o: Simulation.cpp Simulation.h
	g++ -c Simulation.cpp
clean:
	rm *.o executeable