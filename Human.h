//
// Created by nieme on 2021-12-14.
//

#ifndef ASSIGNMENT4_GIDEONNIEMELAINEN_HUMAN_H
#define ASSIGNMENT4_GIDEONNIEMELAINEN_HUMAN_H

#include "Organism.h"

class Human : public Organism
{
public:
    Human();
    Human( City *city, int width, int height);
    Human( City *city, int width, int height, int typeId);
    virtual ~Human();

    // Properties

    // Methods

    void move();
    void turn();
    void recruit();

    void endTurn();

    void move(int target);

    void recruit(int target);
};

#endif //ASSIGNMENT4_GIDEONNIEMELAINEN_HUMAN_H
