// Game --The controller class for AitDoB (interface)

#ifndef GAME_H
#define GAME_H 1

enum class STATE { ERROR = 0, COMMAND, DOOR, QUIT };

class Game {
public:
    static Game* instance();
    ~Game();
    int   run();
    STATE door();
    STATE error();
    STATE move_left();
    STATE move_down();
    STATE move_up();
    STATE move_right();
    STATE move_upleft();
    STATE move_upright();
    STATE move_downleft();
    STATE move_downright();
    STATE move(int row, int col);
    STATE quit();
    STATE refresh();
    STATE resize();
    STATE shell();

private:
    Game();
    static Game* _instance;
};

#endif
