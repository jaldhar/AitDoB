// Ui --The view class for AitDoB (interface)

#ifndef UI_H
#define UI_H 1

#include <cstdint>
#include <map>
#include <memory>
#include <curses.h>

class Game;
enum class TERRAIN : uint8_t;

typedef STATE(Game::*command_ptr)(void);
typedef std::map<int, command_ptr> KEYMAP;
typedef std::map<TERRAIN, chtype> TILEMAP;

class Ui {
public:
    Ui()=default;
    ~Ui()=default;
    void  draw();
    void  end();
    STATE handleInput();
    void  init();
    void  message(const char *msg);
    void  pause();
    void  resize();
    void  refresh();
    void  setMessageWin(WINDOW*& win);
    void  setStatusWin(WINDOW*& win);
    void  shell();

private:
    void  drawMessage();
    void  drawStatus();
    void  drawViewport();

    static WINDOW*    _message;
    static WINDOW*    _status;
    static WINDOW*    _viewport;
    static KEYMAP     _keybindings;
    static TILEMAP    _tilemap;
    static int        _lines;
    static int        _cols;
    static int        _top;
    static int        _left;
};

#endif
