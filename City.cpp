//
// Created by nieme on 2021-12-14.
//

#include <windows.h>
#include "City.h"
#include "GameSpecs.h"
#include "Human.h"
#include "Zombie.h"
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <cstdlib>

void Col(int c)
{
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);
    return;
}

City::City() {
    for (int row = 0; row < GRID_WIDTH; row++) {
        for (int col = 0; col < GRID_HEIGHT; col++) {
            grid[row][col] = nullptr;
        }
    }

    populate();

    // For testing solos

    Human *human =  new Human(this, 2, 2, 1);
    Human *h2 =  new Human(this, 0, 1, 1);
    Zombie *zombie=  new Zombie(this, 4, 4, 2);
    grid[2][2] = human;
    grid[0][1] = h2;
    grid[4][4] = zombie;



}

City::~City() {

}

// For filling the city with 200 humans and 5 zombies to start
void City::populate() {
    // Populating city with base information
    for (int i = 0; i < HUMAN_STARTCOUNT; i++) {
        // Used to randomly generate coordinates
        srand(chrono::system_clock::now().time_since_epoch().count());
        int row = rand() % 20;
        int col = rand() % 20;
        Human *human =  new Human(this, row, col, 1);

        if (this->getOrganism(row, col) == nullptr) {
            grid[row][col] = human;
        } else {
            // Perpetually loop through until required amounts of humans on the board
            i--;
            continue;
        }
    }
    for (int i = 0; i < ZOMBIE_STARTCOUNT; i++) {
        // Used to randomly generate coordinates
        srand(chrono::system_clock::now().time_since_epoch().count());
        int row = rand() % 20;
        int col = rand() % 20;
        Zombie *zombie =  new Zombie(this, row, col, 2);
        // cout << "Value of grid at X: " << row << " Y: " << col << ":  "  << grid[row][col];
        //Human human1 = (const Human &) this->getOrganism(row, col);
        if (this->getOrganism(row, col) == nullptr) {
            grid[row][col] = zombie;
        } else {
            i--;
            continue;
        }
    }

}

// For printing out grid
ostream &operator<<(ostream &os, City &city) {
    // This is temporary for more deliberate testing
    for (int row = 0; row < GRIDSIZE; row++) {
        for (int col = 0; col < GRIDSIZE; col++) {
            if (city.grid[row][col] != nullptr) {
                if(city.grid[row][col]->getTypeId() == 1) {
                    Col(HUMAN_COLOR);
                    os << "H ";
                    //os << row <<"," <<col << "H ";
                } else {
                    Zombie *z = dynamic_cast<Zombie*>(city.getOrganism(row, col));
                    Col(ZOMBIE_COLOR);
                    os << "Z ";
                    //os << row <<"," <<col << "Z ";
                    //os << city.grid[row][col]->getTimer() << "Z ";
                    //os << z->getFeedTimer() << "Z ";
                }
            } else {
                Col(7);
                //os << city.grid[row][col];
                os << "- ";
            }
        }
        Col(7);
        os << "\n";
    }
    cout << "Human Count: " << city.countType(1) << " Zombie Count: " << city.countType(2) << " Total Organisms: " << city.countOrganisms() << endl;
    cout << "Iteration: " << city.turns << endl;

    return os;
}

// Getter and Setter
Organism *City::getOrganism(int row, int col) {
    if (grid[row][col] == nullptr) {
        return nullptr;
    } else {
        return grid[row][col];
    }
}

void City::setOrganism(Organism *organism, int row, int col) {
    grid[row][col] = organism;
}

// Moving the different beings
void City::move() {
    // Vector for possible targets
   humansMove();
   zombiesMoveEat();
}

void City::humansMove() {
    // Declare vector for potential locations
    vector<int> openSquares;
    // Move down one row
    for (int row = 0; row < GRIDSIZE; row++) {
        // move through every column of said row
        for (int col = 0; col < GRIDSIZE; col++) {
            // Clear every time
            openSquares.clear();
            // if something that hasnt moved is there
            if (grid[row][col] != nullptr && !grid[row][col]->hasMoved()) {
                // if its a human that hasnt moved
                if (grid[row][col]->getTypeId() == 1 && !grid[row][col]->hasMoved()) {
                    Human *h = dynamic_cast<Human *>(getOrganism(row, col));
                    openSquares = lookAroundHuman(row, col); // Getting vector of possible targets
                    shuffle(openSquares.begin(), openSquares.end(), default_random_engine(col)); // shuffling vector
                    // For selecting a randomized move target
                    srand(chrono::system_clock::now().time_since_epoch().count());

                   int size = openSquares.size();
                   int moveTarget;
                   int index;
                   if (size == 0) {
                       moveTarget = 0;
                   } else {
                       index = rand() % size - 1;
                       if (index < 0) {
                           index = 0;
                       }
                       moveTarget = openSquares[index];
                   }
                    if (moveTarget > 0) {
                        h->move(moveTarget);
                        setOrganism(h, h->getWidth(), h->getHeight());
                        // delete grid[row][col];
                        grid[row][col] = nullptr;
                    }
                }
            }
        }
    }
}

void City::zombiesMoveEat() {
    // Zombies eat the human in the cell, taking their place
    vector<int> openSquares;
    for (int row = 0; row < GRIDSIZE; row++) {
        for (int col = 0; col < GRIDSIZE; col++) {
            openSquares.clear();
            // if something there that hasnt moved
            if (grid[row][col] != nullptr && !grid[row][col]->hasMoved()) {
                // if something there is a zombie that hasnt moved
                if (grid[row][col]->getTypeId() == 2 && !grid[row][col]->hasMoved()) {
                    Zombie *z = dynamic_cast<Zombie*>(getOrganism(row, col));
                    int feedTimer = z->getFeedTimer();
                    // Look first for possible humans surrounding zombie
                    openSquares = locateHumans(row, col); // Getting vector of possible targets

                    // If no humans found, move
                    if (openSquares.empty()) {
                        openSquares = lookAround(row, col);
                        shuffle(openSquares.begin(),openSquares.end(),default_random_engine(col)); // shuffling vector
                        // For selecting a randomized move target
                        srand(chrono::system_clock::now().time_since_epoch().count());

                        int size = openSquares.size();
                        int moveTarget;
                        int index;
                        if (size == 0) {
                            moveTarget = 0;
                        } else {
                            index = rand() % size - 1;
                            if (index < 0) {
                                index = 0;
                            }
                            moveTarget = openSquares[index];
                        }
                        if (moveTarget > 0) {
                            z->move(moveTarget);
                            z->addToFeedTimer();
                            setOrganism(z, z->getHeight(), z->getWidth());
                            //delete grid[row][col];
                            grid[row][col] = nullptr;
                        }


                        // Else humans were found, so eat
                    } else {
                        shuffle(openSquares.begin(),openSquares.end(),default_random_engine(col)); // shuffling vector
                        // For selecting a randomized move target
                        srand(chrono::system_clock::now().time_since_epoch().count());

                        int size = openSquares.size();
                        int feedTarget;
                        int index;
                        if (size == 0) {
                            feedTarget = 0;
                        } else {
                            index = rand() % size - 1;
                            if (index < 0) {
                                index = 0;
                            }
                            feedTarget = openSquares[index];
                        }
                        // if there is a viable target
                        if (feedTarget > 0 && !z->isFed()) {
                            z->move(feedTarget);
                            grid[z->getWidth()][z->getHeight()] = nullptr; // Removing the human
                            z->feed();
                            setOrganism(z, z->getWidth(), z->getHeight());
                           // delete grid[row][col];
                            grid[row][col] = nullptr;
                            //cout << "Zombie has eaten a human!" << endl;
                        }
                    }
                }
            }
        }
    }
}

// This seems to be working fine now
void City::humansRecruit() {
    vector<int> openSquares;
    // Humans recruit if there is an available space and their internal timer reaches three
    for (int row = 0; row < GRIDSIZE; row++) {
        for (int col = 0; col < GRIDSIZE; col++) {
            openSquares.clear();
            // if something there and hasnt moved
            if (grid[row][col] != nullptr && !grid[row][col]->hasMoved()) {
                // of something there is a human that hasnt moved with a timer set long enough
                if (grid[row][col]->getTypeId() == 1 && !grid[row][col]->hasMoved() && grid[row][col]->getTimer() >= 3) {
                    Human *h = dynamic_cast<Human*>(getOrganism(row, col));
                    openSquares = lookAroundHuman(row, col); // Getting vector of possible targets

                    shuffle(openSquares.begin(),openSquares.end(),default_random_engine(col)); // shuffling vector
                    // For selecting a randomized move target
                    srand(chrono::system_clock::now().time_since_epoch().count());


                    int size = openSquares.size();
                    int moveTarget;
                    int index;
                    if (size == 0) {
                        moveTarget = 0;
                    } else {
                        index = rand() % size - 1;
                        if (index < 0) {
                            index = 0;
                        }
                        moveTarget = openSquares[index];
                    }

                    if (h->getTimer() >= 3) {
                        setOrganism(h, h->getWidth(), h->getHeight());
                        h->move(moveTarget);
                        setOrganism(h, h->getWidth(), h->getHeight());
                        //cout << "A human has been recruited. Power to the cause!" << endl;
                    }
                }
            }
        }
    }
    // If no space available, reset timer to zero
}

void City::zombiesRecruit() {
    vector<int> humans;
    // for every row
    for (int row = 0; row < GRIDSIZE; row++) {
        // for every column in row
        for (int col = 0; col < GRIDSIZE; col++) {
            // clear out potentially convertible humans from last go around
            humans.clear();
            // if something there
            if (grid[row][col] != nullptr) {
                // of something there is a zombie
                if (grid[row][col]->getTypeId() == 2) {
                    Zombie *z = dynamic_cast<Zombie*>(getOrganism(row, col));
                    if (z->isReady()) {
                        humans = locateHumans(row, col); // Getting vector of possible targets
                        if (!humans.empty()) {
                            shuffle(humans.begin(),humans.end(),default_random_engine(col)); // shuffling vector
                            // For selecting a randomized move target
                            srand(chrono::system_clock::now().time_since_epoch().count());
                            int size = humans.size();
                            int convTarget;
                            int index;
                            if (size == 0) {
                                convTarget = 0;
                            } else {
                                index = rand() % size - 1;
                                if (index < 0) {
                                    index = 0;
                                }
                                convTarget = humans[index];
                            }
                            if (convTarget > 0) {
                                // Turn human in new square into nullptr
                                z->move(convTarget);
                                grid[z->getWidth()][z->getHeight()] = nullptr;
                                setOrganism(z, z->getWidth(), z->getHeight());
                                z->hasConverted();
                                z->feed();
                               // cout << "A human has been converted to a zombie. Less power to the cause!" << endl;
                            }
                        }

                    }
                }
            }
        }
    }
    // If a zombie survives for 8 steps, it will convert an adjacent human into a zombie.
    // If they reach this plateau, maintain this state until they encounter a human
}

void City::zombiesStarve() {
    // If zombie goes three turns without eating, it converts back to human
    for (int row = 0; row < GRIDSIZE; row++) {
        for (int col = 0; col < GRIDSIZE; col++) {
            // if something there and hasnt moved
            if (grid[row][col] != nullptr) {
                // if something there is a zombie
                if (grid[row][col]->getTypeId() == 2) {
                    Zombie *z = dynamic_cast<Zombie*>(getOrganism(row, col));
                    // If zombie timer is long enough
                    if (z->getFeedTimer() >= 3) { // Because it basically starts at 1
                        Human *h = new Human(this, row, col, 1);
                        //delete grid[row][col];
                        grid[row][col] = nullptr;
                        setOrganism(h, h->getWidth(), h->getHeight());
                        //cout << "A Zombie has been converted to a Human. A miraculous recovery, I hope her husband hasn't moved on yet!!" << endl;
                    };
                }
            }
        }
    }
}







void City::reset() {
    turns = 0;
    for (int i = 0; i < GRIDSIZE; i++) {
        for (int j = 0; j < GRIDSIZE; j++) {
            grid[i][j] = nullptr;
        }
    }
    populate();
}

int City::countOrganisms() {
    int counter;
    for (int i = 0; i < GRIDSIZE; i++) {
        for (int j = 0; j < GRIDSIZE; j++) {
            if (grid[i][j] != nullptr) {
                counter++;
            }
        }
    }
    return counter;
}

int City::getGeneration() {
    return this->turns;
}

int City::countType(int typeId) {
    int counter = 0;
    for (int i = 0; i < GRIDSIZE; i++) {
        for (int j = 0; j < GRIDSIZE; j++) {
            if (grid[i][j] != nullptr) {
                if (grid[i][j]->getTypeId() == typeId) {
                    counter++;
                } else {
                    continue;
                }
            }
        }
    }
    return counter;
}

void City::endTurn() {
    turns += 1;
    for (int row = 0; row < GRIDSIZE; row++) {
        for (int col = 0; col < GRIDSIZE; col++) {
            if (grid[row][col] != nullptr) {
                if (grid[row][col]->getTypeId() == 1 ) {
                    Human *h = dynamic_cast<Human*>(getOrganism(row, col));
                    h->endTurn();
                } else if (grid[row][col]->getTypeId() == 2 ) {
                    Zombie *z = dynamic_cast<Zombie*>(getOrganism(row, col));
                    z->endTurn();
                }
            }
        }
    }
}

bool City::hasDiversity() {
    if (countType(1) > 0 && countType(2) > 0) {
        return true;
    } else {
        return false;
    }
}

// For getting a vector of human targets
vector<int> City::locateHumans(int row, int col) {
    vector<int> targets;
    if (row == 0) {
        // if on X border and col top
        if (col == 0) {
            if (grid[row + 1][col] != nullptr) { // stacked if statements to avoid segmentation faults
                if (grid[row + 1][col]->getTypeId() == 1) {
                    targets.push_back(5);
                }
            } if (grid[row + 1][col + 1] != nullptr) {
                if (grid[row + 1][col + 1]->getTypeId() == 1) {
                    targets.push_back(6);
                }

            } if (grid[row][col + 1] != nullptr && grid[row][col + 1]->getTypeId() == 1) {
                targets.push_back(7);
            }
            return targets;

            // X border and col bottom
        } else if (col == 19) {
            if (grid[row][col - 1] != nullptr) {
                if (grid[row][col - 1]->getTypeId() == 1) {
                    targets.push_back(3);
                }
            } if (grid[row + 1][col - 1] != nullptr) {
                if (grid[row + 1][col - 1]->getTypeId() == 1) {
                    targets.push_back(4);
                }
            } if (grid[row + 1][col] != nullptr) {
                if (grid[row + 1][col]->getTypeId() == 1) {
                    targets.push_back(5);
                }
            }
            return targets;
        } else {
            if (grid[row][col - 1] != nullptr) {
                if (grid[row][col - 1]->getTypeId() == 1) {
                    targets.push_back(3);
                }
            } if (grid[row + 1][col - 1] != nullptr) {
                if (grid[row + 1][col - 1]->getTypeId() == 1) {
                    targets.push_back(4);
                }
            } if (grid[row + 1][col] != nullptr) {
                if (grid[row + 1][col]->getTypeId() == 1) {
                    targets.push_back(5);
                }
            } if (grid[row + 1][col + 1] != nullptr) {
                if (grid[row + 1][col + 1]->getTypeId() == 1) {
                    targets.push_back(6);
                }
            } if (grid[row][col + 1] != nullptr && grid[row][col + 1]->getTypeId() == 1) {
                if (grid[row][col + 1]->getTypeId() == 1) {
                    targets.push_back(7);
                }
            }
            return targets;
        }
        // If at the far right border
    }
    else if (row == 19){
        if (col == 0) {
            if (grid[row - 1][col] != nullptr) {
                if (grid[row - 1][col]->getTypeId() == 1) {
                    targets.push_back(1);
                }

            } if (grid[row][col + 1] != nullptr) {
                if (grid[row][col + 1]->getTypeId() == 1) {
                    targets.push_back(7);
                }

            } if (grid[row - 1][col + 1] != nullptr) {
                if (grid[row - 1][col + 1]->getTypeId() == 1) {
                    targets.push_back(8);
                }

            }
            return targets;

        } else if (col == 19) {
            if (grid[row - 1][col] != nullptr && grid[row - 1][col]->getTypeId() == 1) {
                targets.push_back(1);
            } if (grid[row - 1][col - 1] != nullptr && grid[row - 1][col - 1]->getTypeId() == 1) {
                targets.push_back(2);
            } if (grid[row][col - 1] != nullptr && grid[row][col - 1]->getTypeId() == 1) {
                targets.push_back(3);
            }
            return targets;

        } else {
            if (grid[row - 1][col] != nullptr && grid[row - 1][col]->getTypeId() == 1) {
                targets.push_back(1);
            } if (grid[row - 1][col - 1] != nullptr && grid[row - 1][col - 1]->getTypeId() == 1) {
                targets.push_back(2);
            } if (grid[row][col - 1] != nullptr && grid[row][col - 1]->getTypeId() == 1) {
                targets.push_back(3);
            } if (grid[row][col + 1] != nullptr && grid[row][col + 1]->getTypeId() == 1) {
                targets.push_back(7);
            } if (grid[row - 1][col + 1] != nullptr && grid[row - 1][col + 1]->getTypeId() == 1) {
                targets.push_back(8);
            }
            return targets;

        }
    } else {
        if ((row-1 >= 0) && grid[row - 1][col] != nullptr && grid[row - 1][col]->getTypeId() == 1) {
            targets.push_back(1);
        } if ((row-1 >= 0 && col-1 >= 0) && grid[row - 1][col - 1] != nullptr && grid[row - 1][col - 1]->getTypeId() == 1) {
            targets.push_back(2);
        } if (col-1 >= 0 && grid[row][col - 1] != nullptr && grid[row][col - 1]->getTypeId() == 1) {
            targets.push_back(3);
        } if ((row+1 <= 19 && col-1 >= 0) && grid[row + 1][col - 1] != nullptr && grid[row + 1][col - 1]->getTypeId() == 1) {
            targets.push_back(4);
        } if (row+1 <= 19 &&grid[row + 1][col] != nullptr && grid[row + 1][col]->getTypeId() == 1) {
            targets.push_back(5);
        } if ((row+1 <= 19 && col+1 <= 19) && grid[row + 1][col + 1] != nullptr && grid[row + 1][col + 1]->getTypeId() == 1) {
            targets.push_back(6);
        } if (col+1 <= 19 && grid[row][col + 1] != nullptr && grid[row][col + 1]->getTypeId() == 1) {
            targets.push_back(7);
        } if ((row-1 >= 0 && col+1 <= 19) && grid[row - 1][col + 1] != nullptr && grid[row - 1][col + 1]->getTypeId() == 1) {
            targets.push_back(8);
        }
    }
    return targets;
}
// For getting a vector of possible locations
vector<int> City::lookAround(int row, int col) {
    // Initialize vector to be returned
    vector<int> targets;
    // Test case for 8 squares for occupancy
    // Test for edge
    // This became an absolute horror show of a decision tree
    // if row borders
    if (row == 0) {
        // if on X border and col top
        if (col == 0) {
            if (grid[row + 1][col] == nullptr) {
                targets.push_back(5);
            } if (grid[row + 1][col + 1] == nullptr) {
                targets.push_back(6);
            } if (grid[row][col + 1] == nullptr) {
                targets.push_back(7);
            }
            return targets;

            // X border and col bottom
        } else if (col == 19) {
            if (grid[row][col - 1] == nullptr) {
                targets.push_back(3);
            } else if (grid[row + 1][col - 1] == nullptr) {
                targets.push_back(4);
            } else if (grid[row + 1][col] == nullptr) {
                targets.push_back(5);
            }
            return targets;
        } else {
            if (grid[row][col - 1] == nullptr) {
                targets.push_back(3);
            } if (grid[row + 1][col - 1] == nullptr) {
                targets.push_back(4);
            } if (grid[row + 1][col] == nullptr) {
                targets.push_back(5);
            } if (grid[row + 1][col + 1] == nullptr) {
                targets.push_back(6);
            } if (grid[row][col + 1] == nullptr) {
                targets.push_back(7);
            }
            return targets;
        }
        // If at the far right border
    }
    else if (row == 19){
        if (col == 0) {
            if (grid[row - 1][col] == nullptr) {
                targets.push_back(1);
            } if (grid[row][col + 1] == nullptr) {
                targets.push_back(7);
            } if (grid[row - 1][col + 1] == nullptr) {
                targets.push_back(8);
            }
            return targets;

        } else if (col == 19) {
            if (grid[row - 1][col] != nullptr) {
                targets.push_back(1);
            } if (grid[row - 1][col - 1] == nullptr) {
                targets.push_back(2);
            } if (grid[row][col - 1] == nullptr) {
                targets.push_back(3);
            }
            return targets;

        } else {
            if (grid[row - 1][col] == nullptr) {
                targets.push_back(1);
            } if (grid[row - 1][col - 1] == nullptr) {
                targets.push_back(2);
            } if (grid[row][col - 1] == nullptr) {
                targets.push_back(3);
            } if (grid[row][col + 1] == nullptr) {
                targets.push_back(7);
            } if (grid[row - 1][col + 1] == nullptr) {
                targets.push_back(8);
            }
            return targets;

        }
    }
    else {
        if ((row - 1) >= 0 && grid[row - 1][col] == nullptr) {
            targets.push_back(1);
        } if (((row-1) >= 0 && (col - 1) >= 0) && grid[row - 1][col - 1] == nullptr) {
            targets.push_back(2);
        } if ((col - 1) >= 0 && grid[row][col - 1] == nullptr) {
            targets.push_back(3);
        } if (((row+1) <= 19 && (col - 1) >= 0) && grid[row + 1][col - 1] == nullptr) {
            targets.push_back(4);
        } if ((row+1) <= 19 && grid[row + 1][col] == nullptr) {
            targets.push_back(5);
        } if (((row+1) <= 19 && (col + 1) <= 19) && grid[row + 1][col + 1] == nullptr) {
            targets.push_back(6);
        } if ((col - 1) <= 19 && grid[row][col + 1] == nullptr) {
            targets.push_back(7);
        } if (((row-1) >= 0 && (col + 1) <= 19) && grid[row - 1][col + 1] == nullptr) {
            targets.push_back(8);
        } else {
            targets.push_back(0);
        }
    }
    return targets;
}

vector<int> City::lookAroundHuman(int row, int col) {
    vector<int> targets;
    // Test case for 8 squares for occupancy
    // Test for edge
    // This became an absolute horror show of a decision tree
    // Reminder that width and height are backwards
    // x axis = height(cols), y axis = width(rows)
    // Accessible blocks for humans are 1.left[row-1], 2.up[col-1], 3.right[row+1], 4.down[col+1]
    // if row borders

    if (row == 0) {
        // top and far left
        if (col == 0) {
                       //y     x
            if (grid[row + 1][col] == nullptr) {
                targets.push_back(4);
            } if (grid[row][col + 1] == nullptr) {
                targets.push_back(4);
            }
            return targets;

            // Top and far right
        } else if (col == 19) {
            if (grid[row][col - 1] == nullptr) {
                targets.push_back(2);
            } else if (grid[row + 1][col] == nullptr) {
                targets.push_back(3);
            }
            return targets;
        } else {
            if (grid[row][col - 1] == nullptr) {
                targets.push_back(2);
            } if (grid[row + 1][col] == nullptr) {
                targets.push_back(3);
            }  if (grid[row][col + 1] == nullptr) {
                targets.push_back(4);
            }
            return targets;
        }

    }

    else if (row == 19){
        // bottom and far left
        if (col == 0) {
            if (grid[row - 1][col] == nullptr) {
                targets.push_back(1);
            } if (grid[row][col + 1] == nullptr) {
                targets.push_back(4);
            }
            return targets;

        } else if (col == 19) {
            if (grid[row - 1][col] != nullptr) {
                targets.push_back(1);
            }  if (grid[row][col - 1] == nullptr) {
                targets.push_back(2);
            }
            return targets;

        } else {
            if (grid[row - 1][col] == nullptr) {
                targets.push_back(1);
            }  if (grid[row][col - 1] == nullptr) {
                targets.push_back(2);
            } if (grid[row][col + 1] == nullptr) {
                targets.push_back(4);
            }
            return targets;

        }
    }
    else { // Need to add boundary checking for moving squares
        if ((row - 1) >= 0 && grid[row - 1][col] == nullptr) {
            targets.push_back(1);
        } if ((col - 1) >= 0 && grid[row][col - 1] == nullptr) {
            targets.push_back(2);
        }  if ((row + 1) <= 19 && grid[row + 1][col] == nullptr) {
            targets.push_back(3);
        } if ((col + 1) <= 19 && grid[row][col + 1] == nullptr) {
            targets.push_back(4);
        }  else {
            targets.push_back(0);
        }
    }

    return targets;
}


