// World --The model class for AitDoB (interface)

#ifndef WORLD_H
#define WORLD_H 1

#include <vector>
class Corridor;
class Player;
class Room;
class Tile;

class World {
public:
    static World* instance();
    ~World();
    void          create();
    int           height();
    int           width();
    int           sectorHeight();
    int           sectorWidth();
    Player&       player();
    int           playerRow();
    void          setPlayerRow(int row);
    int           playerCol();
    void          setPlayerCol(int col);
    Tile&         tileAt(int row, int col);

private:
    World();
    void          addCorridors();
    void          addDoors();
    void          addWalls();
    void          buildRooms(int sectorRows, int sectorCols);

    static const int MAP_HEIGHT       = 60;
    static const int MAP_WIDTH        = 80;
    static const int MAP_SECTORHEIGHT = 20;
    static const int MAP_SECTORWIDTH  = 20;
    Tile*                           _map[MAP_HEIGHT][MAP_WIDTH];
    std::vector<Corridor*>          _corridors;
    std::vector<Room*>              _rooms;
    Player*                         _player;
    int                             _playerRow;
    int                             _playerCol;
    static World*                   _instance;

};

#endif
