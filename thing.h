// Thing -- An object of some sort in AitDoB (interface)
//

#ifndef THING_H
#define THING_H 1

enum class THINGTYPE { NOTHING = 0, DOOR };

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
