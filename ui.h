// Ui --The view class for AitDoB (interface)

#ifndef UI_H
#define UI_H 1

#include <map>
#include <curses.h>

class Game;

class Ui {
public:
    static Ui* instance();
    ~Ui();
    void  draw();
    void  end();
    STATE handleInput();
    void  init();
    void  pause();
    void  resize();
    void  refresh();
    void  setStatusWin(WINDOW *&win);
    void  setTitleWin(WINDOW *&win);
    void  shell();

private:
    Ui();
    void  drawStatus();
    void  drawTitle();
    void  drawViewport();

    typedef STATE(Game::*command_ptr)(void);
    typedef std::map<int, command_ptr> KEYMAP;
    WINDOW*            _status;
    WINDOW*            _title;
    WINDOW*            _viewport;
    KEYMAP             _keybindings;
    int                _lines;
    int                _cols;
    int                _top;
    int                _left;
    static Ui*         _instance;
};

#endif
