// World --The model class for AitDoB (implementation)

#include <algorithm>
#include <cstdlib>
using namespace std;

#include "corridor.h"
#include "door.h"
#include "player.h"
#include "room.h"
#include "tile.h"
#include "world.h"

World* World::_instance = nullptr;

World* World::instance() {
    if(_instance == nullptr)
        _instance = new World();

    return _instance;
}

World::World() {
}

World::~World() {
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            if(_map[row][col] != 0)
                delete _map[row][col];
        }
    }
    delete [] _map;
   for_each(_corridors.begin(), _corridors.end(), [](Corridor *p){ delete p; });
   for_each(_rooms.begin(), _rooms.end(), [](Room *p){ delete p; });

    if(_player != nullptr)
        delete _player;

    if(_instance != nullptr)
        delete _instance;
}

void World::create() {
    // Begin by filling in the entire grid.
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            _map[row][col] = new Tile();
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
    for_each(_rooms.begin(), _rooms.end(), [](Room *r) { r->fill(); });

    // Add walls.
    addWalls();

    // Add doors.
    addDoors();

    // Position player.
    _player = new Player();
    do {
        _playerRow = rand() % MAP_HEIGHT;
        _playerCol = rand() % MAP_WIDTH;
    } while (!_map[_playerRow][_playerCol]->passable());
}

void World::addCorridors() {
    // Create corridors by shuffling the list of rooms and going backwards
    // through it joining the center point of each room to that of the room
    // before it.
    random_shuffle(_rooms.begin(), _rooms.end());
    vector<Room*>::reverse_iterator rit;
    for (rit = _rooms.rbegin(); rit < _rooms.rend() - 1; ++rit) {
        Room*& r1 = *rit;
        Room*& r2 = *(rit + 1); // previous room in list.
        _corridors.push_back(new Corridor(r1->centerX(),r1->centerY(),
            r2->centerX(),r2->centerY()));
    }
    // Join the last and the first rooms to complete the loop.
    Room*& r1 = *(_rooms.begin());
    Room*& r2 = *(_rooms.end() - 1);
    _corridors.push_back(new Corridor(r1->centerX(),r1->centerY(),
        r2->centerX(),r2->centerY()));
}

void World::addDoors() {
    for (int row = 0; row < MAP_HEIGHT; row++) {
        for (int col = 0; col < MAP_WIDTH; col++) {
            Tile*& t = _map[row][col];
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
            Tile*& t = _map[row][col];
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
            Tile*& t = _map[row][col];
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
            _rooms.push_back(new Room(top, left, height, width));
        }
    }
}

int World::height() {
    return MAP_HEIGHT;
}

int World::width() {
    return MAP_WIDTH;
}

int World::sectorHeight() {
    return MAP_SECTORHEIGHT;
}

int World::sectorWidth() {
    return MAP_SECTORWIDTH;
}

Player& World::player() {
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

Tile& World::tileAt(int row, int col) {
    return *_map[row][col];
}
