// Game --The controller class for AitDoB (interface)

#ifndef GAME_H
#define GAME_H 1

typedef enum { ERROR = 0, COMMAND, QUIT } STATE;

class Game {
public:
    static Game* instance();
    ~Game();
    int   run();
    STATE error();
    STATE quit();
    STATE refresh();
    STATE resize();
    STATE shell();

private:
    Game();
    static Game* _instance;
};

#endif
