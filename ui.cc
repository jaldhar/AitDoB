// Ui --The view class for AitDoB (implementation)

#include <clocale>
#include <csignal>
#include <cstdlib>
#include <cstring>
using namespace std;

#include "game.h"
#include "player.h"
#include "tile.h"
#include "ui.h"
#include "world.h"

WINDOW*            Ui::_message;
WINDOW*            Ui::_status;
WINDOW*            Ui::_viewport;
KEYMAP             Ui::_keybindings;
TILEMAP            Ui::_tilemap;
int                Ui::_lines;
int                Ui::_cols;
int                Ui::_top;
int                Ui::_left;

static void end_sig(int /* sig */) {
    Ui ui;
    ui.end();
}

static int createStatusWin(WINDOW* win, int /* ncols */) {
    Ui ui;
    ui.setStatusWin(win);

    return 0;
}

static int createMessageWin(WINDOW* win, int /* ncols */) {
    Ui ui;
    ui.setMessageWin(win);

    return 0;
}

void Ui::draw() {
    curs_set(0);
    drawViewport();
    drawStatus();
    drawMessage();
    refresh();
}

void Ui::drawStatus() {
    World world;
    Player& p = world.player();
    mvwprintw(_status, 0, 1, "H: %2d    XP: %2d",
        p.health(), p.xp());
    wnoutrefresh(_status);
}

void Ui::drawMessage() {
    wnoutrefresh(_message);
}

void Ui::drawViewport() {
    int height, width;

    wclear(_viewport);
    getmaxyx(_viewport, height, width);
    World world;
    int playerRow = world.playerRow();
    int playerCol = world.playerCol();

    int top = playerRow - (height / 2);
    if (top < 0) {
        top = 0;
    }
    int left = playerCol - (width / 2);
    if (left < 0) {
        left = 0;
    }
    for (int row = 0; row < height; row++) {
        wmove(_viewport, row, 0);
        int mapRow = row + top;
        if (mapRow > world.height() - 1) {
            continue;
        }
        for (int col = 0; col < width; col++) {
            int mapCol = col + left;
            if (mapCol > world.width() - 1) {
                continue;
            }
            chtype display;
            if (mapRow == playerRow && mapCol == playerCol) {
                display = '@' | COLOR_PAIR(5);
            }
            else {
                Tile& t = world.tileAt(mapRow, mapCol);
                    display = _tilemap[t.terrain()];
                }
            }
            waddch(_viewport, display);
        }
    }
    wnoutrefresh(_viewport);
}

void Ui::end() {
    curs_set(1);
    endwin();
    clear();
    exit(EXIT_SUCCESS);
}


STATE Ui::handleInput() {
    int c = getch();
    auto it = _keybindings.find(c);
    if (it != _keybindings.end()) {
        Game game;
        return (game.*it->second)();
    }
    beep();
    return STATE::ERROR;
}

void Ui::init() {
    setlocale(LC_ALL, "POSIX");
    signal (SIGINT, end_sig);
    signal (SIGSEGV, end_sig);

    _viewport = 0;
    _lines = 0;
    _cols  = 0;
    _top   = 0;
    _left  = 0;
    nonl();
    cbreak();
    noecho();
    _keybindings[0x12] /* CTRL-R */ = &Game::refresh;
    _keybindings['Q']               = &Game::quit;
    _keybindings['!']               = &Game::shell;
    _keybindings['h']               = &Game::move_left;
    _keybindings[KEY_LEFT]          = &Game::move_left;
    _keybindings['j']               = &Game::move_down;
    _keybindings[KEY_DOWN]          = &Game::move_down;
    _keybindings['k']               = &Game::move_up;
    _keybindings[KEY_UP]            = &Game::move_up;
    _keybindings['l']               = &Game::move_right;
    _keybindings[KEY_RIGHT]         = &Game::move_right;
    _keybindings['y']               = &Game::move_upleft;
    _keybindings['u']               = &Game::move_upright;
    _keybindings['b']               = &Game::move_downleft;
    _keybindings['n']               = &Game::move_downright;
    _keybindings[KEY_RESIZE]        = &Game::resize;

    ripoffline(1, createMessageWin);
    ripoffline(-1, createStatusWin);
    initscr();
    keypad(stdscr, TRUE);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);
        init_pair(3, COLOR_BLACK,  COLOR_CYAN);
        init_pair(4, COLOR_BLACK,  COLOR_BLACK);
        init_pair(5, COLOR_RED,  COLOR_WHITE);
    }

    _tilemap[TERRAIN::EMPTY]    = ' '; // use of ACS_* requires this goes
    _tilemap[TERRAIN::CORRIDOR] = '.'; // after call to initscr().
    _tilemap[TERRAIN::H_DOOR]   = 'x';
    _tilemap[TERRAIN::V_DOOR]   = 'x';
    _tilemap[TERRAIN::FLOOR]    = '.';
    _tilemap[TERRAIN::C_WALL]   = '+';
    _tilemap[TERRAIN::H_WALL]   = ACS_HLINE;
    _tilemap[TERRAIN::V_WALL]   = ACS_VLINE;
    _tilemap[TERRAIN::UL_WALL]  = ACS_ULCORNER;
    _tilemap[TERRAIN::UR_WALL]  = ACS_URCORNER;
    _tilemap[TERRAIN::LR_WALL]  = ACS_LRCORNER;
    _tilemap[TERRAIN::LL_WALL]  = ACS_LLCORNER;
    _tilemap[TERRAIN::TT_WALL]  = ACS_TTEE;
    _tilemap[TERRAIN::RT_WALL]  = ACS_RTEE;
    _tilemap[TERRAIN::BT_WALL]  = ACS_BTEE;
    _tilemap[TERRAIN::LT_WALL]  = ACS_LTEE;

    resize();
}

void Ui::message(const char *msg) {
    wclear(_message);
    mvwaddstr(_message, 0, 0, msg);
    wrefresh(_message);
}

void Ui::pause() {
    getch();
}

void Ui::refresh() {
    wrefresh(_status);
    wrefresh(_message);
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

    wresize(_message, 1, _cols);
    wbkgd(_message, ' ' | COLOR_PAIR(1));

    wresize(_status, 1, _cols);
    wbkgd(_status, ' ' | COLOR_PAIR(3));

    draw();
}

void Ui::setStatusWin(WINDOW*& win) {

    _status = win;
}

void Ui::setMessageWin(WINDOW*& win) {

    _message = win;
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
