//
// Created by Matúš on 03/01/2024.
//

#include "Cell.h"


Cell::Cell() {
    this->biotope = 'X';
    this->burningTime = 0;
}

char Cell::getBiotope() {
    return this->biotope;
};

void Cell::setBiotope(char biotope) {
    this->biotope=biotope;
}

int Cell::getBurningTime() {
    return this->burningTime;
}

void Cell::setBurningTime(int time) {
    this->burningTime=time;
}

Cell::Cell(char biotop, int burnTime) {
    this->biotope = biotop;
    this->burningTime = burnTime;
}


