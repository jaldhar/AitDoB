// Corridor -- A corridor in AitDoB (interface)
//

#ifndef CORRIDOR_H
#define CORRIDOR_H 1

class Corridor {
public:
    Corridor(int ax, int ay, int bx, int by);
private:
    void makeHorizontal(int row1, int row2, int col);
    void makeVertical(int row, int col1, int col2);
};

#endif
