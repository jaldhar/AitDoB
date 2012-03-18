// Thing -- An object of some sort in AitDoB (implementation)
//

#include "thing.h"

Thing::Thing() {
    _type = THINGTYPE::NOTHING;
}

Thing::Thing(THINGTYPE type) {
    _type = type;
}

Thing* Thing::clone() {
    return new Thing(*this);
}

THINGTYPE Thing::type() {
    return _type;
}

void Thing::setType(THINGTYPE type) {
    _type = type;
}

