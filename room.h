// Room -- A room in AitDoB (interface)
//

#ifndef ROOM_H
#define ROOM_H 1

class Room {
public:
    Room(int top, int left, int height, int width);
    void fill();
    int  top();
    int  left();
    int  height();
    int  width();
    int  centerX();
    void setCenterX(int x);
    int  centerY();
    void setCenterY(int y);

private:
    int _top, _left, _height, _width, _centerX, _centerY;
};

#endif
