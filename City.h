//
// Created by nieme on 2021-12-14.
//

#ifndef ASSIGNMENT4_GIDEONNIEMELAINEN_CITY_H
#define ASSIGNMENT4_GIDEONNIEMELAINEN_CITY_H

#include <iostream>
#include <vector>

using namespace std;

class Organism;

const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;

class City
{
protected:
    Organism *grid[GRID_HEIGHT][GRID_WIDTH];
    int turns;
public:
    City();
    virtual ~City();

    Organism *getOrganism(int row, int col );
    void setOrganism(Organism *organism, int row, int col );

    void populate();
    void move();
    void humansMove();
    void zombiesMoveEat();
    void humansRecruit();
    void zombiesRecruit();
    void zombiesStarve();
    void reset();
    bool hasDiversity();
    int countOrganisms();
    int getGeneration();
    int countType(int typeId);
    void endTurn();

    friend ostream& operator<<( ostream &output, City &city );

    //int lookAround(int row, int col);
    vector<int> lookAround(int row, int col);
    vector<int>lookAroundHuman(int row, int col);

    vector<int> locateHumans(int row, int col);
};

#endif //ASSIGNMENT4_GIDEONNIEMELAINEN_CITY_H
