// Combat -- AitDoB combat system (interface)
//

#ifndef COMBAT_H
#define COMBAT_H 1

class Combat {
public:
    Combat(int health = 0, int offense = 0, int defense = 0);
    int  attack();
    int  defend();
    int  defense();
    void setDefense(int n);
    int  health();
    void setHealth(int n);
    int  offense();
    void setOffense(int n);
protected:
    int _health;
    int _offense;
    int _defense;
};

#endif
