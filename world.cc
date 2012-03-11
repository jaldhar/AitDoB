// World --The model class for AitDoB (implementation)

#include "world.h"

World* World::_instance = 0;

World* World::instance() {
    if(_instance == 0)
        _instance = new World();

    return _instance;
}

World::World() {
}

World::~World() {
    if(_instance != 0)
        delete _instance;
}


