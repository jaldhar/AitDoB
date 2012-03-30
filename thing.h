// Thing -- An object of some sort in AitDoB (interface)
//

#ifndef THING_H
#define THING_H 1

#include <cstdint>

enum class THINGTYPE : uint8_t { NOTHING = 0, DOOR };

class Thing {
public:
    Thing();
    Thing(THINGTYPE type);
    virtual     ~Thing() { };
    virtual     Thing* clone();
    THINGTYPE   type();
    void        setType(THINGTYPE type);

protected:
    THINGTYPE   _type;
};

#endif
