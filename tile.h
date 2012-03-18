// Tile -- A tile in AitDoB (interface)
//

#ifndef TILE_H
#define TILE_H 1

enum class TERRAIN { EMPTY = 0, CORRIDOR, H_DOOR, V_DOOR, FLOOR, C_WALL, H_WALL,
    V_WALL, UL_WALL, UR_WALL, LR_WALL, LL_WALL, TT_WALL, RT_WALL, BT_WALL,
    LT_WALL };

class Thing;

class Tile {
public:
    Tile();
    ~Tile();
    Thing*  contents();
    void    setContents(Thing* thing);
    bool    passable();
    void    setPassable(bool p);
    TERRAIN terrain();
    void    setTerrain(TERRAIN t);
    bool    isWall();

private:
    Thing*  _contents;
    bool    _passable;
    TERRAIN _terrain;
};

#endif
