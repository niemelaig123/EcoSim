//
// Created by nieme on 2021-12-14.
//

#include "Organism.h"

Organism::Organism(City *city, int width, int height) {

}

Organism::Organism() {

}

Organism::Organism(City *city, int width, int height, int typeId, bool moved)
:city(city), width(width), height(height), typeId(typeId), moved(true)
{

}

Organism::Organism(City *city, int width, int height, int typeId)
:city(city), width(height), x(height), y(width), height(width), typeId(typeId)
{

}
void Organism::setPosition(int x, int y) {

}

void Organism::endTurn() {
    this->moved = true;
}


ostream &operator<<(ostream &os, Organism *organism) {
    os << "Organism height: " << organism->height << " Organism width: " << organism->width << endl;
    return os;
}

Organism::~Organism() {

}

int Organism::getTypeId() {
    return this->typeId;
}

int Organism::getHeight() {
    return this->height;
}

int Organism::getWidth() {
    return this->width;
}

bool Organism::hasMoved() {
    if (this->moved) {
        return true;
    } else {
        return false;
    }

}

int Organism::getTimer() {
    return this->orgTimer;
}

void Organism::reset() {
    this->moved = false;
}

int Organism::lookAround() {
    return 0;
}

void Organism::turn() {

}

void Organism::move(int target) {

}

void Organism::setTimer(int timer) {
    this->orgTimer = timer;
}




