//
// Created by Matúš on 03/01/2024.
//

#include "Simulation.h"

static uniform_real_distribution<double> uniformDistribution(0,1);
static default_random_engine rng;

Simulation::Simulation(int size, double probabilityForTrees, double probabilityForWater, double probabilityPlains, double probabilityForRocks) {
    this->playFieldSize=size;
    this->windCountDown=0;
    this->wind="X";
    this->hold = false;
    this->run = true;


    rng.seed(std::chrono::system_clock::now().time_since_epoch().count());

    this->playField.resize(this->playFieldSize);
    for (int i = 0; i < this->playFieldSize; ++i) {
        this->playField.at(i).resize(this->playFieldSize);
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            double probability = uniformDistribution(rng);
            if (probability < probabilityForTrees) {
                this->playField.at(i).at(j).setBiotope('T');
            } else if (probability < (probabilityForTrees + probabilityForWater)) {
                this->playField.at(i).at(j).setBiotope('W');
            } else if (probability < (probabilityForTrees + probabilityForWater + probabilityPlains)) {
                this->playField.at(i).at(j).setBiotope('P');
            } else if (probability < (probabilityForTrees + probabilityForWater + probabilityPlains + probabilityForRocks)) {
                this->playField.at(i).at(j).setBiotope('R');
            }
        }
    }
}

Simulation::~Simulation() {
}

void Simulation::simulate() {

    vector<vector<Cell>> tempVector;

    while (this->run) {


        Sleep(1000);

        if (tempVector.size() != this->playFieldSize) {
            tempVector.resize(this->playFieldSize);
            for (int i = 0; i < this->playFieldSize; ++i) {
                tempVector.at(i).resize(this->playFieldSize);
            }
        }

        if (this->wind == "X") {
            generateWind();
        }

        unique_lock<mutex> lock(this->mutVar);

        for (int i = 0; i < this->playFieldSize; ++i) {
            for (int j = 0; j < this->playFieldSize; ++j) {

                Cell currentCell = this->playField.at(i).at(j);
                tempVector.at(i).at(j)= Cell(currentCell.getBiotope(),currentCell.getBurningTime());

                if (currentCell.getBiotope() == 'T') {
                    if (setCellAflame(i,j)) {
                        tempVector.at(i).at(j).setBiotope('F');
                        tempVector.at(i).at(j).setBurningTime(3);
                    }

                } else if (currentCell.getBiotope() == 'P') {
                    if (setCellAflame(i,j)) {
                        tempVector.at(i).at(j).setBiotope('F');
                        tempVector.at(i).at(j).setBurningTime(2);
                    } else if (turnCellIntoForest(i,j)) {
                        tempVector.at(i).at(j).setBiotope('T');
                    }

                } else if (currentCell.getBiotope() == 'F') {
                    if (currentCell.getBurningTime() == 1) {
                        tempVector.at(i).at(j).setBiotope('B');
                    }
                    tempVector.at(i).at(j).setBurningTime(currentCell.getBurningTime()-1);

                } else if (this->playField.at(i).at(j).getBiotope() == 'B') {
                    if (turnCellIntoPlain(i,j)) {
                        tempVector.at(i).at(j).setBiotope('P');
                    }
                }
            }
        }

        for (int i = 0; i < this->playFieldSize; ++i) {
            for (int j = 0; j < this->playFieldSize; ++j) {
                this->playField.at(i).at(j).setBiotope(tempVector.at(i).at(j).getBiotope());
                this->playField.at(i).at(j).setBurningTime(tempVector.at(i).at(j).getBurningTime());
            }
        }

        lock.unlock();

        printOut();

        if (this->wind != "X") {
            this->windCountDown--;
            if (this->windCountDown == 0) {
                this->wind = 'X';
            }
        }

    }
}

void Simulation::printOut() {
    cout << "Wind direction: " << this->wind << endl << "|0|";
    for (int i = 1; i <= this->playFieldSize; ++i) {
        cout << i << '|';
    }
    cout << endl;

    for (int i = 0; i < this->playFieldSize; ++i) {
        cout << '|' << i+1 << '|';
        for (int j = 0; j < this->playFieldSize; ++j) {
            cout << this->playField.at(i).at(j).getBiotope() << '|';
        }
        cout << endl;
    }
    cout<<"---------------------------------------------------------------" << endl;
    cout<<"To hold simulation type \'h\'" << endl;
    cout<<"---------------------------------------------------------------" << endl;
}

void Simulation::generateWind() {
    if (uniformDistribution(rng) < 0.1) {
        double windDirection = uniformDistribution(rng);
        if (windDirection < 0.25) {
            this->wind='N';
        } else if (windDirection < 0.5) {
            this->wind='E';
        } else if (windDirection < 0.75) {
            this->wind='S';
        } else {
            this->wind='W';
        }
        this->windCountDown = 3;
    }
}

bool Simulation::turnCellIntoPlain(int Y, int X) {
    bool turnIntoPlain= false;

    if (X-1 >= 0) {
        if (this->playField.at(Y).at(X-1).getBiotope() == 'W' && uniformDistribution(rng) < 0.10) {
            turnIntoPlain = true;
        }
    }

    if (X+1 < this->playFieldSize) {
        if (this->playField.at(Y).at(X+1).getBiotope() == 'W' && uniformDistribution(rng) < 0.10) {
            turnIntoPlain = true;
        }
    }

    if (Y-1 >= 0) {
        if (this->playField.at(Y-1).at(X).getBiotope() == 'W' && uniformDistribution(rng) < 0.10) {
            turnIntoPlain = true;
        }
    }

    if (Y+1 < this->playFieldSize) {
        if (this->playField.at(Y+1).at(X).getBiotope() == 'W' && uniformDistribution(rng) < 0.10) {
            turnIntoPlain = true;
        }
    }

    return turnIntoPlain;
}

bool Simulation::turnCellIntoForest(int Y, int X) {
    bool turnIntoForest = false;

    if (X-1 >= 0) {
        if (this->playField.at(Y).at(X-1).getBiotope() == 'T' && uniformDistribution(rng) < 0.02) {
            turnIntoForest = true;
        }
    }

    if (X+1 < this->playFieldSize) {
        if (this->playField.at(Y).at(X+1).getBiotope() == 'T' && uniformDistribution(rng) < 0.02) {
            turnIntoForest = true;
        }
    }

    if (Y-1 >= 0) {
        if (this->playField.at(Y-1).at(X).getBiotope() == 'T' && uniformDistribution(rng) < 0.02) {
            turnIntoForest = true;
        }
    }

    if (Y+1 < this->playFieldSize) {
        if (this->playField.at(Y+1).at(X).getBiotope() == 'T' && uniformDistribution(rng) < 0.02) {
            turnIntoForest = true;
        }
    }

    return turnIntoForest;
}

bool Simulation::setCellAflame(int Y, int X) {
    bool setAflame = false;

    if (X-1 >= 0) {
        if (this->playField.at(Y).at(X-1).getBiotope() == 'F') {
            if (this->wind=="W" && uniformDistribution(rng) < 0.9) {
                setAflame = true;
            } else if (this->wind=="E" && uniformDistribution(rng) < 0.02) {
                setAflame = true;
            } else if ( this->wind=="X" && uniformDistribution(rng) < 0.2) {
                setAflame = true;
            }
        }
    }

    if (X+1 < this->playFieldSize) {
        if (this->playField.at(Y).at(X+1).getBiotope() == 'F') {
            if (this->wind=="E" && uniformDistribution(rng) < 0.9){
                setAflame = true;
            } else if (this->wind=="W" && uniformDistribution(rng) < 0.02) {
                setAflame = true;
            } else if ( this->wind=="X" && uniformDistribution(rng) < 0.2) {
                setAflame = true;
            }
        }
    }

    if (Y-1 >= 0) {
        if (this->playField.at(Y-1).at(X).getBiotope() == 'F') {
            if (this->wind=="N" && uniformDistribution(rng) < 0.9){
                setAflame = true;
            } else if (this->wind=="S" && uniformDistribution(rng) < 0.02) {
                setAflame = true;
            } else if ( this->wind=="X" && uniformDistribution(rng) < 0.2) {
                setAflame = true;
            }
        }
    }

    if (Y+1 < this->playFieldSize) {
        if (this->playField.at(Y+1).at(X).getBiotope() == 'F') {
            if (this->wind=="S" && uniformDistribution(rng) < 0.9){
                setAflame = true;
            } else if (this->wind=="N" && uniformDistribution(rng) < 0.02) {
                setAflame = true;
            } else if ( this->wind=="X" && uniformDistribution(rng) < 0.2) {
                setAflame = true;
            }
        }
    }

    return setAflame;
}

void Simulation::listenInput() {
    io_service io_service;
    tcp::socket socket(io_service);
    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query("frios2.fri.uniza.sk","http");
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::ip::tcp::endpoint endpoint = iter->endpoint();
    socket.connect( tcp::endpoint( endpoint.address(), 17319));
    while (this->run) {
        string input;
        cin >> input;
        if (input == "h") {
            unique_lock<mutex> lock(this->mutVar);
            cout<<"To end simulation type \":e\"" << endl;
            cout<<"To continue simulation type \":cont\"" << endl;
            cout<<"To set cells aflame type \":set\"" << endl;
            cout<<"To save wold on server type \":s\"" << endl;
            cout<<"To download wold from server type \":d\"" << endl;
            cin >> input;
            if (input == ":cont") {
                lock.unlock();
            }
            if (input == ":set") {
                cout<<"How many cells do you wish to set aflame?" << endl;
                int cellsAflame;
                cin >> cellsAflame;
                setCellsAflameManually(cellsAflame);
                lock.unlock();
            }
            if (input == ":e") {
                this->run = false;
                write("end", socket);
            }
            if (input == ":s") {
                write(this->playingFieldToString(), socket);
            }
            if (input == ":d") {
                write("download", socket);
                cout << read(socket);
                cin >> input;
                write(input, socket);
                replace(read(socket));
            }
        }

    }
}

void Simulation::replace(string message) {

    vector<string> splitWindAndCells = split(message, '/');

    this->wind = split(splitWindAndCells.at(0),':').at(0);
    this->windCountDown = stoi(split(splitWindAndCells.at(0),':').at(1));

    vector<string> splitRows= split(splitWindAndCells.at(1), '|');

    this->playFieldSize = (int)splitRows.size();
    this->playField.resize(this->playFieldSize);
    for (int i = 0; i < this->playFieldSize; ++i) {
        this->playField.at(i).resize(this->playFieldSize);
        vector<string> row = split(splitRows.at(i),';');
        for (int j = 0; j < this->playFieldSize; ++j) {
            vector<string> cell = split(row.at(j),':');
            this->playField.at(i).at(j).setBiotope(cell[0][0]);
            this->playField.at(i).at(j).setBurningTime(stoi(cell[1]));
        }
    }


}

vector<string> Simulation::split(std::string stringToParse, char delimeter) {
    vector<string> parsedString;

    string temp;

    stringstream ss(stringToParse);

    while (getline(ss, temp, delimeter))
        parsedString.push_back(temp);

    return parsedString;
}

string Simulation::playingFieldToString() {
    stringstream stream;

    stream << this->wind << ':' << this->windCountDown << '/';

    for (int i = 0; i < this->playFieldSize; ++i) {
        for (int j = 0; j < this->playFieldSize; ++j) {
            stream << this->playField.at(i).at(j).getBiotope() << ':';
            stream << this->playField.at(i).at(j).getBurningTime() << ';';
        }
        stream << '|';
    }
    return stream.str();
}

void Simulation::setCellsAflameManually(int numberOfCells) {
    for (int i = 0; i < numberOfCells; ++i) {
        cout<<"Type coordinates of " << i+1 << ". cell in format \"X Y\" " << endl;
        int X;
        int Y;
        bool checkCondition = true;
        while (checkCondition) {
            cin >> X;
            cin >> Y;
            if (X < 1 || X > this->playFieldSize || Y < 1 || Y > this->playFieldSize) {
                cout << "Coordinates out of bounds. Try again." << endl;
            } else if (this->playField.at(Y-1).at(X-1).getBiotope() != 'T' && this->playField.at(Y-1).at(X-1).getBiotope() != 'P') {
                cout << "Biotope not flammable. Try again.";
            } else {
                this->playField.at(Y-1).at(X-1).setBiotope('F');
                if (this->playField.at(Y-1).at(X-1).getBiotope() == 'P') {
                    this->playField.at(Y-1).at(X-1).setBurningTime(2);
                } else {
                    this->playField.at(Y-1).at(X-1).setBurningTime(3);
                }
                checkCondition = false;
            }
        }
    }
}

string Simulation::read(tcp::socket & socket) {
    boost::asio::streambuf buf;
    read_until( socket, buf, "\n" );
    string data = buffer_cast<const char*>(buf.data());
    data.pop_back();
    return data;
}

void Simulation::write(string stringToWrite, tcp::socket & socket) {
    const string msg = stringToWrite + "\n";
    boost::asio::write(socket, boost::asio::buffer(msg));
}



