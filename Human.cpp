//
// Created by nieme on 2021-12-14.
//

#include <chrono>
#include "Human.h"
#include "Organism.h"

Human::Human(City *city, int width, int height)
: Organism(city, width, height) {
    this->city = city;
}

Human::Human(City *city, int width, int height, int typeId) : Organism(city, width, height, typeId)
{
    this->city = city;
    this->height = height;
    this->width = width;
    this->typeId = typeId;
    this->orgTimer = 0;
}

Human::Human() {

}

Human::~Human() = default;

void Human::move() {
    srand(chrono::system_clock::now().time_since_epoch().count());
    int dir = rand() % 4;
    switch (dir){
        case 1: // Left
            this->width -= 1;
            this->moved = true;
            break;
        case 2: // Up
            this->height -= 1;
            this->moved = true;
            break;
        case 3: // Right
            this->width += 1;
            this->moved = true;
            break;
        case 4: // Down
            this->height += 1;
            this->moved = true;
            break;
    }
}

// New move function, takes in a randomly generated integer, but is evaluated in City code for viability first (ie checking if the square is taken or not)
void Human::move(int target) {
    switch (target){
        // need to put boundary guards on here too
        case 1: // Left
            this->width -= 1;
            if (this->width <= 0) {
                this->width = 0;
            }

            this->moved = true;
            break;
        case 2: // Up
            this->height -= 1;
            if (this->height <= 0) {
                this->height = 0;
            }
            this->moved = true;
            break;
        case 3: // Right
            this->width += 1;
            if (this->width >= 19) {
                this->width = 19;
            }

            this->moved = true;
            break;
        case 4: // Down
            this->height += 1;
            if (this->height >= 19) {
                this->height = 19;
            }
            this->moved = true;
            break;
        default:
            this->moved = true;
            break;
    }
}

void Human::recruit(int target) {
    switch (target) {
        case 1: // Left
            this->width -= 1;
            break;
        case 2: // Up
            this->height -= 1;
            break;
        case 3: // Right
            this->width += 1;
            break;
        case 4: // Down
            this->height += 1;
            break;
        default:
            break;
    }

}

void Human::turn() {
    orgTimer += 1;
    if (orgTimer == 3) {
        orgTimer = 0;
    }
    endTurn();
}


void Human::endTurn() {
    if (this->orgTimer == 3) {
        this->orgTimer = 0;
    }
    this->orgTimer += 1;
    this->moved = false;
    // Some redundancy
    if (this->height < 0) {
        this->height = 0;
    }
    if (this->height > 19) {
        this->height = 19;
    }
    if (this->width < 0) {
        this->width = 0;
    }
    if (this->width > 19) {
        this->width = 19;
    }
    this->x = this->height;
    this->y = this->width;
}

