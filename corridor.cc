// Corridor -- A corridor in AitDoB (implementation)
//

#include <cstdlib>
using namespace std;

#include "corridor.h"
#include "tile.h"
#include "world.h"

extern World* world;

Corridor::Corridor(int ax, int ay, int bx, int by) {
    if (ax < bx) {
        makeVertical(ax, bx, by);
        if (ay < by)                    // a to nw of b
            makeHorizontal(ax, ay, by);
        else                            // a to ne of b
            makeHorizontal(ax, by, ay);
    }
    else {
        makeVertical(bx, ax, by);
        if (ay < by)                    // a to sw of b
            makeHorizontal(ax, ay, by);
        else
            makeHorizontal(ax, by, ay); // a to se of b
   }

}

void Corridor::makeHorizontal(int row, int col1, int col2) {
    for (int i = col1; i <= col2; i++) {
        Tile& t = world->tileAt(row, i);
//        if (t.terrain() == TERRAIN::CORRIDOR && i != col1 && i != col2) {
//            break;
//        }
//        else {
            t.setPassable(true);
            t.setTerrain(TERRAIN::CORRIDOR);
//        }
    }
}

void Corridor::makeVertical(int row1, int row2, int col) {
    for (int i = row1; i <= row2; i++) {
        Tile& t = world->tileAt(i, col);
//        if (t.terrain() == TERRAIN::CORRIDOR && i != row1 && i != row2) {
//            break;
//        }
//        else {
            t.setPassable(true);
            t.setTerrain(TERRAIN::CORRIDOR);
        }
//    }
}
