// Corridor -- A corridor in AitDoB (implementation)
//

#include <cstdlib>
using namespace std;

#include "door.h"
#include "tile.h"
#include "world.h"

extern World* world;

Door::Door(int row, int col, bool horizontal) : Thing(THINGTYPE::DOOR),
    Combat(1, 1, 1) {
    _row = row;
    _col = col;
    Tile& t = world->tileAt(row, col);
    t.setTerrain(horizontal ? TERRAIN::H_DOOR : TERRAIN::V_DOOR);
    _open = false;
}

bool Door::open() {
    return _open;
}

void Door::setOpen(bool open) {
    _open = open;
}

int Door::row() {
    return _row;
}

int Door::col() {
    return _col;
}
