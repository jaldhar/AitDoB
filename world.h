// World --The model class for AitDoB (interface)

#ifndef WORLD_H
#define WORLD_H 1

class World {
public:
    static World* instance();
    ~World();

private:
    World();
    static World*                   _instance;

};

#endif
