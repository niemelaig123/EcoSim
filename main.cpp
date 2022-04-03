// C++ Assignment 4 - Eco Sim
// By Gideon Niemelainen


#include <iostream>
#include <windows.h>
#include "City.h"
#include "Organism.h"
#include "Zombie.h"
#include <chrono>
#include <thread>

using namespace std;

void Col(int);
// Handy quick function to clear the screen of output
void ClearScreen()
{
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

int main() {
    // Program variables
    long INTERVAL = 1000;
    // Declare City and set Timer
    City *city = new City();
    chrono:: milliseconds interval(INTERVAL);

    cout << *city;
    while (city->hasDiversity()) {
        this_thread::sleep_for(interval);
        ClearScreen();

        bool diverse;
        city->humansMove();
        city->zombiesMoveEat();
        city->humansRecruit();
        city->zombiesRecruit();
        city->zombiesStarve();
        city->endTurn();
        diverse = city->hasDiversity();
        if (diverse) {
            //cout << "Wow how diverse!" << endl;
        }
        cout << *city;
    }
    return 0;
}

