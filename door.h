// Door -- A door in AitDoB (interface)
//

#ifndef DOOR_H
#define DOOR_H 1

#include "thing.h"
#include "combat.h"

class Door : public Thing, public Combat {
public:
    Door(int row, int col, bool horizontal);
    bool open();
    void setOpen(bool open);
    int  row();
    int  col();

private:
    int     _row;
    int     _col;
    bool    _open;
};

#endif
