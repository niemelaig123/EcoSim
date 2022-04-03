//
// Created by nieme on 2021-12-14.
//

#ifndef ASSIGNMENT4_GIDEONNIEMELAINEN_ORGANISM_H
#define ASSIGNMENT4_GIDEONNIEMELAINEN_ORGANISM_H

#include <iostream>

using namespace std;

class City;

class Organism
{
protected:
    int x;
    int y;
    int width;
    int height;
    int typeId;
    int orgTimer;
    bool moved = false;
    City *city;

    enum { WEST, NORTH, EAST, SOUTH, NUM_DIRECTIONS };

public:
    Organism();
    Organism( City *city, int width, int height);
    Organism( City *city, int width, int height, int typeId );
    Organism(City *city, int width, int height, int typeId, bool moved);
    virtual ~Organism();

    // Properties


    // Methods
    virtual void move() = 0;
    //virtual void spawn() = 0;
    //virtual int getSpecies() = 0; //this could also be coded concrete here
    //virtual void getPosition() = 0;

    void setPosition( int x, int y );
    int lookAround();
    int getTypeId();
    int getHeight();
    int getWidth();
    int getTimer();
    void setTimer(int timer);
    void endTurn();
    void reset();
    bool hasMoved();

    // This is gonna be the prime mover
    void turn();
    void move(int target);

    friend ostream& operator<<( ostream &output, Organism *organism );

};

#endif //ASSIGNMENT4_GIDEONNIEMELAINEN_ORGANISM_H
