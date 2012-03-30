// World --The model class for AitDoB (interface)

#ifndef WORLD_H
#define WORLD_H 1

#include <array>
#include <functional>
#include <memory>
#include <vector>

class Player;
class Room;
class Tile;
class World;

typedef std::unique_ptr<Player> PLAYERPTR;
typedef const std::unique_ptr<Player>& PLAYERPTRREF;
typedef std::unique_ptr<Room> ROOMPTR;
typedef const std::unique_ptr<Room>& ROOMPTRREF;
typedef std::unique_ptr<Tile> TILEPTR;
typedef const std::unique_ptr<Tile>& TILEPTRREF;

static const int MAP_HEIGHT       = 60;
static const int MAP_WIDTH        = 80;
static const int MAP_SECTORHEIGHT = 20;
static const int MAP_SECTORWIDTH  = 20;

class World {
public:
    World()=default;
    ~World()=default;
    void          create();
    int           height() const;
    int           width() const;
    int           sectorHeight() const;
    int           sectorWidth() const;
    Player&       player() const;
    int           playerRow();
    void          setPlayerRow(int row);
    int           playerCol();
    void          setPlayerCol(int col);
    Tile&         tileAt(int row, int col) const;
private:
    void          addCorridors();
    void          makeCorridor(int ax, int ay, int bx, int by);
    void          makeHorizontalCorridor(int row1, int row2, int col);
    void          makeVerticalCorridor(int row, int col1, int col2);
    void          addDoors();
    void          addWalls();
    void          buildRooms(int sectorRows, int sectorCols);

    static std::array<std::array<TILEPTR, MAP_WIDTH>,MAP_HEIGHT> _map;
    static std::vector<ROOMPTR>            _rooms;
    static PLAYERPTR                       _player;
    static int                             _playerRow;
    static int                             _playerCol;
};

#endif
