//
// Created by nieme on 2021-12-14.
//

#ifndef ASSIGNMENT4_GIDEONNIEMELAINEN_ZOMBIE_H
#define ASSIGNMENT4_GIDEONNIEMELAINEN_ZOMBIE_H

#include "Organism.h"

class Zombie : public Organism
{
protected:
    bool readyConvert;
    bool fed;
    int feedTimer;
public:
    Zombie();
    Zombie( City *city, int width, int height );
    Zombie( City *city, int width, int height, int typeId );
    virtual ~Zombie();

    // Properties

    // Methods

    void move();
    void move(int target);
    void endTurn();
    int getFeedTimer();
    void addToFeedTimer();
    bool isFed();
    void feed();
    bool isReady();
    void hasConverted();
};


#endif //ASSIGNMENT4_GIDEONNIEMELAINEN_ZOMBIE_H
