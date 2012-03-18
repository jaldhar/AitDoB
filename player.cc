// Player -- The player in AitDoB (implementation)
//

#include "player.h"

Player::Player() : Combat(20, 1, 1) {
    _potions = 0;
    _xp = 0;
}

Player* Player::clone() {
    return new Player(*this);
}

int  Player::potions() {
    return _potions;
}

void Player::addPotion(int n) {
    _potions += n;
}

int  Player::xp() {
    return _xp;
}

void Player::addXp(int n) {
    _xp += n;
}
