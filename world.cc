// World --The model class for AitDoB (implementation)

#include <algorithm>
#include <cstdlib>
using namespace std;

#include "door.h"
#include "player.h"
#include "room.h"
#include "tile.h"
#include "world.h"

array<array<TILEPTR, MAP_WIDTH>,MAP_HEIGHT> World::_map;
vector<ROOMPTR>                 World::_rooms;
PLAYERPTR                       World::_player;
int                             World::_playerRow;
int                             World::_playerCol;

void World::create() {
    // Begin by filling in the entire grid.
    for (array<TILEPTR, MAP_WIDTH>& row : _map) {
        for (TILEPTR& col : row) {
            col = TILEPTR(new Tile());
        }
    }

    // Divide the grid into sectors.
    int sectorRows = MAP_HEIGHT / MAP_SECTORHEIGHT;
    int sectorCols = MAP_WIDTH / MAP_SECTORWIDTH;

    // Build a room in each sector.
    buildRooms(sectorRows, sectorCols);

    // Add corridors.
    addCorridors();

    // Now add the rooms. This goes after addCorridors() to prevent collisions.
    for(ROOMPTRREF r : _rooms)
        r->fill();

    // Add walls.
    addWalls();

    // Add doors.
    addDoors();

    // Position player.
    _player = PLAYERPTR(new Player());
    do {
        _playerRow = rand() % MAP_HEIGHT;
        _playerCol = rand() % MAP_WIDTH;
    } while (!_map[_playerRow][_playerCol]->passable());
}

void World::addCorridors() {
    // Create corridors by shuffling the list of rooms and going backwards
    // through it joining the center point of each room to that of the room
    // before it.
    random_shuffle(begin(_rooms), end(_rooms));
    for (auto rit = _rooms.rbegin(); rit < _rooms.rend() - 1; ++rit) {
        ROOMPTRREF r1 = *rit;
        ROOMPTRREF r2 = *(rit + 1); // previous room in list.
        makeCorridor(r1->centerX(),r1->centerY(),r2->centerX(),r2->centerY());
    }
    // Join the last and the first rooms to complete the loop.
    ROOMPTRREF r1 = *(begin(_rooms));
    ROOMPTRREF r2 = *(end(_rooms) - 1);
    makeCorridor(r1->centerX(),r1->centerY(),r2->centerX(),r2->centerY());
}

void World::makeCorridor(int ax, int ay, int bx, int by) {
    if (ax < bx) {
        makeVerticalCorridor(ax, bx, by);
        if (ay < by)                    // a to nw of b
            makeHorizontalCorridor(ax, ay, by);
        else                            // a to ne of b
            makeHorizontalCorridor(ax, by, ay);
    }
    else {
        makeVerticalCorridor(bx, ax, by);
        if (ay < by)                    // a to sw of b
            makeHorizontalCorridor(ax, ay, by);
        else
            makeHorizontalCorridor(ax, by, ay); // a to se of b
   }

}

void World::makeHorizontalCorridor(int row, int col1, int col2) {
    for (int i = col1; i <= col2; i++) {
        TILEPTRREF t = _map[row][i];
        t->setPassable(true);
        t->setTerrain(TERRAIN::CORRIDOR);
    }
}

void World::makeVerticalCorridor(int row1, int row2, int col) {
    for (int i = row1; i <= row2; i++) {
        TILEPTRREF t = _map[i][col];
        t->setPassable(true);
        t->setTerrain(TERRAIN::CORRIDOR);
    }
}

void World::addDoors() {
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            TILEPTRREF t = _map[row][col];
            if (t->terrain() != TERRAIN::CORRIDOR)
                continue;
            if(_map[row - 1][col]->isWall() && _map[row + 1][col]->isWall() &&
                (_map[row][col - 1]->terrain() == TERRAIN::FLOOR ||
                _map[row][col + 1]->terrain() == TERRAIN::FLOOR)) {
                t->setContents(new Door(row, col, false));
            }
            else if(_map[row][col-1]->isWall() && _map[row][col+1]->isWall() &&
                (_map[row - 1][col]->terrain() == TERRAIN::FLOOR ||
                _map[row + 1][col]->terrain() == TERRAIN::FLOOR)) {
                t->setContents(new Door(row, col, true));
            }
        }
    }
}

void World::addWalls() {
    //First pass puts a center wall around everything
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            TILEPTRREF t = _map[row][col];
            if (t->terrain() != TERRAIN::EMPTY)
                continue;
            for (int x = row - 1; x < row + 2; x++) {
                if (x < 0 || x > MAP_HEIGHT - 1)
                    continue;
                for (int y = col - 1; y < col + 2; y++) {
                    if (y < 0 || y > MAP_WIDTH - 1)
                        continue;
                    if (x == row && y == col)
                        continue;
                    TERRAIN c = _map[x][y]->terrain();
                    if (c == TERRAIN::FLOOR || c == TERRAIN::CORRIDOR) {
                        t->setTerrain(TERRAIN::C_WALL);
                        t->setPassable(false);
                        goto end;
                    }
                }
            }
            end: ;
        }
    }
    // Second pass makes specific wall types as needed.
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            TILEPTRREF t = _map[row][col];
            if (t->terrain() != TERRAIN::C_WALL)
                continue;
            int count = 7, edges = 0; // represent the edges as a binary number
            for (int x = row - 1; x < row + 2; x++) {
                if (x < 0 || x > MAP_HEIGHT - 1) {
                    count -= 3;
                    continue;
                }
                for (int y = col - 1; y < col + 2; y++) {
                    if (y < 0 || y > MAP_WIDTH - 1) {
                        count--;
                        continue;
                    }
                    if (x == row && y == col)
                        continue;
                    if (_map[x][y]->isWall()) {
                        edges += pow(2, count);
                    }
                    count--;
                }
            }

            if ((edges & 90) == 90)              // = 01011010
                t->setTerrain(TERRAIN::C_WALL);
            else if ((edges & 88) == 88)         // = 01011000
                t->setTerrain(TERRAIN::BT_WALL);
            else if ((edges & 82) == 82)         // = 01010010
                t->setTerrain(TERRAIN::RT_WALL);
            else if ((edges & 74) == 74)         // = 01001010
                t->setTerrain(TERRAIN::LT_WALL);
            else if ((edges & 26) == 26)         // = 00011010
                t->setTerrain(TERRAIN::TT_WALL);
            else if ((edges & 80) == 80)         // = 01010000
                t->setTerrain(TERRAIN::LR_WALL);
            else if ((edges & 72) == 72)         // = 01001000
                t->setTerrain(TERRAIN::LL_WALL);
            else if ((edges & 18) == 18)         // = 00010010
                t->setTerrain(TERRAIN::UR_WALL);
            else if ((edges & 10) == 10)         // = 00001010
                t->setTerrain(TERRAIN::UL_WALL);
            else if ((edges & 66) == 66 || (edges & 64) == 64 ||
                (edges & 2) == 2)                // = 01000010,01000000,00000010
                t->setTerrain(TERRAIN::V_WALL);
            else if ((edges & 24) == 24 || (edges & 16) == 16 ||
                (edges & 8) == 8)                // = 00011000,00010000,00001000
                t->setTerrain(TERRAIN::H_WALL);
            else if ((edges & 0) == 0)           // = 00000000
                t->setTerrain(TERRAIN::C_WALL);
            else
                fprintf(stderr, "%d\n", edges); // For debugging.
        }
    }
}

void World::buildRooms(int sectorRows, int sectorCols) {

    for (int row = 0; row < sectorRows; row++) {
        for (int col = 0; col < sectorCols; col++) {
            int top = (rand() % (MAP_SECTORHEIGHT / 4)) + 1;
            int left = (rand() % (MAP_SECTORWIDTH / 4)) + 1;
            int height = rand() % (MAP_SECTORHEIGHT - top - 1);
            if (height <= MAP_SECTORHEIGHT / 4)
                height += MAP_SECTORHEIGHT / 4;
            height -= height % 2;
            int width = rand() % (MAP_SECTORWIDTH - left - 1);
            if (width < MAP_SECTORWIDTH / 4)
                width += MAP_SECTORWIDTH / 4;
            width -= width % 2;
            top += (row * MAP_SECTORHEIGHT);
            left += (col * MAP_SECTORWIDTH);
            _rooms.push_back(ROOMPTR(new Room(top, left, height, width)));
        }
    }
}

int World::height() const {
    return MAP_HEIGHT;
}

int World::width() const {
    return MAP_WIDTH;
}

int World::sectorHeight() const {
    return MAP_SECTORHEIGHT;
}

int World::sectorWidth() const {
    return MAP_SECTORWIDTH;
}

Player& World::player() const {
    return *_player;
}

int World::playerRow() {
    return _playerRow;
}

void World::setPlayerRow(int row) {
    _playerRow = row;
}

int World::playerCol() {
    return _playerCol;
}

void World::setPlayerCol(int col) {
    _playerCol = col;
}

Tile& World::tileAt(int row, int col) const {
    return *_map[row][col];
}
