// Player -- The player in AitDoB (interface)
//

#ifndef PLAYER_H
#define PLAYER_H 1

#include "combat.h"

class Player : public Combat {
public:
   Player();
    Player*     clone();
    int         potions();
    void        addPotion(int n);
    int         xp();
    void        addXp(int n);

private:
    int         _potions;
    int         _xp;
};

#endif
