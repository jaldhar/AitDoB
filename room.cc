// Room -- A room in AitDoB (implementation)
//

#include <cstdlib>
using namespace std;

#include "room.h"
#include "tile.h"
#include "world.h"

extern World* world;

Room::Room(int top, int left, int height, int width) {
    _top    = top;
    _left   = left;
    _height = height;
    _width  = width;
    // Select a random point somewhere in the center of each room.
    _centerX = _top + (rand() % int(_height / 2)) + int((float)_height / 4);
    _centerX -= _centerX % 2;
    _centerY = _left + (rand() % int(_width / 2)) + int((float)_width / 4);
    _centerY -= _centerY % 2;
}

void Room::fill() {
    for (int row = _top; row < _top + _height; row++) {
        for (int col = _left; col < _left + _width; col++) {
            Tile& t = world->tileAt(row, col);
            t.setPassable(true);
            t.setTerrain(TERRAIN::FLOOR);
        }
    }
}

int Room::top() {
    return _top;
}

int Room::left() {
    return _left;
}

int Room::height() {
    return _height;
}

int Room::width() {
    return _width;
}

int Room::centerX() {
    return _centerX;
}

void Room::setCenterX(int x) {
    _centerX = x;
}

int Room::centerY() {
    return _centerY;
}

void Room::setCenterY(int y) {
    _centerY = y;
}
