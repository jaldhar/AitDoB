// Ui --The view class for AitDoB (implementation)

#include <clocale>
#include <csignal>
#include <cstdlib>
#include <cstring>
using namespace std;

#include "game.h"
#include "ui.h"

extern Game* game;
extern Ui*   ui;

Ui* Ui::_instance = 0;

Ui* Ui::instance() {
    if(_instance == 0)
        _instance = new Ui();

    return _instance;
}

static void end_sig(int /* sig */) {
    ui->end();
}

static int createStatusWin(WINDOW *win, int /* ncols */) {
    ui->setStatusWin(win);

    return 0;
}

static int createTitleWin(WINDOW *win, int /* ncols */) {
    ui->setTitleWin(win);

    return 0;
}

Ui::Ui() {
    signal (SIGINT, end_sig);
    signal (SIGSEGV, end_sig);

    _viewport = 0;
    _lines = 0;
    _cols  = 0;
    _top   = 0;
    _left  = 0;
}

Ui::~Ui() {
    if(_instance != 0)
        delete _instance;
}

void Ui::draw() {
    curs_set(0);
    drawViewport();
    drawStatus();
    drawTitle();
    refresh();
}

void Ui::drawStatus() {
    wclear(_status);
    wnoutrefresh(_status);
}

void Ui::drawTitle() {
    wclear(_title);
    const char * const titleText = "Ascent into the Depths of Beyond";
    const int len = ::strlen(titleText);
    mvwaddstr(_title, 0, (_cols - len)/2, titleText);
    wnoutrefresh(_title);
}

void Ui::drawViewport() {
}

void Ui::end() {
    curs_set(1);
    endwin();
    clear();
    exit(EXIT_SUCCESS);
}

STATE Ui::handleInput() {
    int c = getch();

    KEYMAP::iterator it = _keybindings.begin();
    if ((it = _keybindings.find(c)) != _keybindings.end())
        return (game->*it->second)();

    beep();
    return ERROR;
}

void Ui::init() {
    setlocale(LC_ALL, "POSIX");
    nonl();
    cbreak();
    noecho();

    _keybindings[0x12] /* CTRL-R */ = &Game::refresh;
    _keybindings['Q']               = &Game::quit;
    _keybindings['!']               = &Game::shell;
//    _keybindings['h']               = &Game::move_left;
//    _keybindings[KEY_LEFT]          = &Game::move_left;
//    _keybindings['j']               = &Game::move_down;
//    _keybindings[KEY_DOWN]          = &Game::move_down;
//    _keybindings['k']               = &Game::move_up;
//    _keybindings[KEY_UP]            = &Game::move_up;
//    _keybindings['l']               = &Game::move_right;
//    _keybindings[KEY_RIGHT]         = &Game::move_right;
//    _keybindings['y']               = &Game::move_upleft;
//    _keybindings['u']               = &Game::move_upright;
//    _keybindings['b']               = &Game::move_downleft;
//    _keybindings['n']               = &Game::move_downright;
    _keybindings[KEY_RESIZE]        = &Game::resize;

    ripoffline(1, createTitleWin);
    ripoffline(-1, createStatusWin);
    initscr();
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);
        init_pair(3, COLOR_BLACK,  COLOR_CYAN);
        init_pair(4, COLOR_BLACK,  COLOR_BLACK);
    }

    resize();
}

void Ui::pause() {
    getch();
}

void Ui::refresh() {
    wrefresh(_status);
    wrefresh(_title);
    wrefresh(_viewport);
}

void Ui::resize() {
    getmaxyx(stdscr, _lines, _cols);

    wbkgd(stdscr, ' ');

    if (!_viewport) {
        _viewport = subwin(stdscr, _lines, _cols, 0, 0);
    }
    else {
        wresize(_viewport, _lines, _cols);
    }
    wbkgd(_viewport, ' ' | COLOR_PAIR(1));

    wresize(_title, 1, _cols);
    wbkgd(_title, ' ' | COLOR_PAIR(2));

    wresize(_status, 1, _cols);
    wbkgd(_status, ' ' | COLOR_PAIR(3));

    draw();
}

void Ui::setStatusWin(WINDOW *&win) {

    _status = win;
}

void Ui::setTitleWin(WINDOW *&win) {

    _title = win;
}

void Ui::shell() {
    def_prog_mode();
    endwin();
    fprintf(stderr, "Type 'exit' to return.\n");
    int returncode;
    returncode = system("/bin/sh");
    returncode += 0; // stops g++ warning for set but unused variable.
    reset_prog_mode();
}
