// Combat -- AitDoB combat system (implementation)
//

#include <cstdlib>
using namespace std;

#include "combat.h"

Combat::Combat(int health, int offense, int defense) {
    _health  = health;
    _offense = offense;
    _defense = defense;
}

int Combat::attack() {
    int hits = 0;

    for(int i = 0; i < _offense; i++ ) {
        if (rand() % 6 == 5) {
            hits++;
        }
    }

    return hits;
}

int Combat::defend() {
    int parries = 0;

    for(int i = 0; i < _defense; i++ ) {
        if (rand() % 6 == 5) {
            parries++;
        }
    }

    return parries;
}

int Combat::defense() {
    return _defense;
}

void Combat::setDefense(int n) {
    _defense += n;
}

int Combat::health() {
    return _health;
}

void Combat::setHealth(int n) {
    _health += n;
}

int Combat::offense() {
    return _offense;
}

void Combat::setOffense(int n) {
    _offense += n;
}
