//
// Created by nieme on 2021-12-14.
//

#include <chrono>
#include "Zombie.h"
#include "Organism.h"

Zombie::Zombie(City *city, int width, int height) : Organism(city, width, height) {
    this->city = city;
}

Zombie::Zombie(City *city, int width, int height, int typeId) : Organism(city, width, height, typeId) {
    this->orgTimer = 0;
    this->feedTimer = 0;
    this->readyConvert = false;
}

Zombie::Zombie() {

}

Zombie::~Zombie() = default;

void Zombie::move() {
    srand(chrono::system_clock::now().time_since_epoch().count());
    int dir = rand() % 8;
    switch (dir){
        case 0:
            this->moved = true;
            break;
        case 1: // Left
            this->width -= 1;
            this->moved = true;
            break;
        case 2: // Left-Up
            this->height -= 1;
            this->width -= 1;
            this->moved = true;
            break;
        case 3: // Up
            this->height = 1;
            this->moved = true;
            break;
        case 4: // Up-Right
            this->height -= 1;
            this->width += 1;
            this->moved = true;
            break;
        case 5: // Right
            this->width += 1;
            this->moved = true;
            break;
        case 6: // Right-Down
            this->height += 1;
            this->width += 1;
            this->moved = true;
            break;
        case 7: // Down
            this->height += 1;
            this->moved = true;
            break;
        case 8: // Down-Left
            this->height += 1;
            this->width -= 1;
            this->moved = true;
            break;
    }
}

void Zombie::move(int target) {
    // need to put boundary guards on here as well
    // Reminder that width and height are backwards
    // x axis = height(cols), y axis = width(rows)
    switch (target){
        case 1: // Left
            this->width -= 1;
            if (this->width <= 0) {
                this->width = 0;
            }
            this->moved = true;
            break;
        case 2: // Left-Up
            this->width -= 1;
            if (this->width <= 0) {
                this->width = 0;
            }
            this->height -= 1;
            if (this->height <= 0) {
                this->height = 0;
            }

            this->moved = true;
            break;
        case 3: // Up
            this->height -= 1;
            if (this->height <= 0) {
                this->height = 0;
            }
            this->moved = true;
            break;
        case 4: // Up-Right
            this->height -= 1;
            if (this->height <= 0) {
                this->height = 0;
            }
            this->width += 1;
            if (this->width >= 19) {
                this->width = 19;
            }
            this->moved = true;
            break;
        case 5: // Right
            this->width += 1;
            if (this->width >= 19) {
                this->width = 19;
            }
            this->moved = true;
            break;
        case 6: // Right-Down
            this->height += 1;
            if (this->height >= 19) {
                this->height = 19;
            }
            this->width += 1;
            if (this->width >= 19) {
                this->width = 19;
            }
            this->moved = true;
            break;
        case 7: // Down
            this->height += 1;
            if (this->height >= 19) {
                this->height = 19;
            }
            this->moved = true;
            break;
        case 8: // Down-Left
            this->height += 1;
            if (this->height >= 19) {
                this->height = 19;
            }
            this->width -= 1;
            if (this->width <= 0) {
                this->width = 0;
            }
            this->moved = true;
            break;
        default:
            break;
    }
}

void Zombie::endTurn() {
    if (this->orgTimer == 8) { // Ready to convert a human to a zombie after 8 turns
        this->readyConvert = true;
        this->orgTimer = 0;
    }
    this->orgTimer += 1;
    this->fed = false;
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


// When zombie is ready to convert, then reset
bool Zombie::isReady() {
    return this->readyConvert;
}

void Zombie::hasConverted() {
    this->readyConvert = false;
}

void Zombie::addToFeedTimer() {
    this->feedTimer += 1;
}

int Zombie::getFeedTimer() {
    return this->feedTimer;
}

void Zombie::feed() {
    this->feedTimer = 0;
    this->fed = true;
    this->moved = true;
}

bool Zombie::isFed() {
    return this->fed;
}




