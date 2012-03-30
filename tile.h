// Tile -- A tile in AitDoB (interface)
//

#ifndef TILE_H
#define TILE_H 1

#include <cstdint>
#include <memory>

enum class TERRAIN : std::uint8_t { EMPTY = 0, CORRIDOR, H_DOOR, V_DOOR, FLOOR,
    C_WALL, H_WALL, V_WALL, UL_WALL, UR_WALL, LR_WALL, LL_WALL, TT_WALL,
    RT_WALL, BT_WALL, LT_WALL };

class Thing;

class Tile {
public:
    Tile();
    ~Tile()=default;
    Thing&  contents() const;
    void    setContents(Thing* thing);
    bool    passable();
    void    setPassable(bool p);
    TERRAIN terrain();
    void    setTerrain(TERRAIN t);
    bool    isWall();

private:
    std::unique_ptr<Thing>  _contents;
    bool    _passable;
    TERRAIN _terrain;
};

#endif
