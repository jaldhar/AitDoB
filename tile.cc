// Tile -- A tile in AitDoB (implementation)
//

#include "thing.h"
#include "tile.h"

Tile::Tile() {
    _contents = nullptr;
    _passable = false;
    _visible = false;
    _terrain = TERRAIN::EMPTY;
}

Thing& Tile::contents() const {
    return *_contents;
}

void Tile::setContents(Thing* thing) {
    if (thing == nullptr) {
        delete _contents.release();
        _contents.reset(nullptr);
    }
    else
        _contents.reset(thing);
}

bool Tile::visible() {
    return _visible;
}

void Tile::setVisible(bool v) {
    _visible = v;
}

bool Tile::passable() {
    return _passable;
}

void Tile::setPassable(bool p) {
    _passable = p;
}

TERRAIN Tile::terrain() {
    return _terrain;
}

void Tile::setTerrain(TERRAIN t) {
    _terrain = t;
}

bool Tile::isBlock() {
    return (_terrain == TERRAIN::H_WALL || _terrain == TERRAIN::V_WALL ||
    _terrain == TERRAIN::UL_WALL || _terrain == TERRAIN::UR_WALL ||
    _terrain == TERRAIN::LR_WALL || _terrain == TERRAIN::LL_WALL ||
    _terrain == TERRAIN::TT_WALL || _terrain == TERRAIN::RT_WALL ||
    _terrain == TERRAIN::BT_WALL || _terrain == TERRAIN::LT_WALL ||
    _terrain == TERRAIN::C_WALL  ||
    _terrain == TERRAIN::H_DOOR  || _terrain == TERRAIN::V_DOOR);
}
