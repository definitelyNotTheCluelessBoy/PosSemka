//
// Created by Matúš on 03/01/2024.
//

#ifndef SEMESTRALKACLIENT_CELL_H
#define SEMESTRALKACLIENT_CELL_H


class Cell {
private:
    char biotope;
    int burningTime;
public:
    Cell(char biotop, int burnTime);
    Cell();
    char getBiotope();
    void setBiotope(char biotope);
    int getBurningTime();
    void setBurningTime(int time);
};


#endif //SEMESTRALKACLIENT_CELL_H
