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

extern Game*  game;
extern Ui*    ui;
extern World* world;

Ui* Ui::_instance = nullptr;

Ui* Ui::instance() {
    if(_instance == nullptr)
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

static int createMessageWin(WINDOW *win, int /* ncols */) {
    ui->setMessageWin(win);

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

    if(_message)            // These were created with malloc(3)
        free(_message);
    if(_status)
        free(_status);
    if(_viewport)
        free(_viewport);

    if(_instance != nullptr)
        delete _instance;
}

void Ui::draw() {
    curs_set(0);
    drawViewport();
    drawStatus();
    drawMessage();
    refresh();
}

void Ui::drawStatus() {
    Player& p = world->player();
    wclear(_status);
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

    int top = world->playerRow() - (height / 2);
    if (top < 0) {
        top = 0;
    }
    int left = world->playerCol() - (width / 2);
    if (left < 0) {
        left = 0;
    }
    for (int row = 0; row < height; row++) {
        wmove(_viewport, row, 0);
        int mapRow = row + top;
        if (mapRow > world->height() - 1) {
            continue;
        }
        for (int col = 0; col < width; col++) {
            int mapCol = col + left;
            if (mapCol > world->width() - 1) {
                continue;
            }
            chtype display;
            int playerRow = world->playerRow();
            int playerCol = world->playerCol();
            if (mapRow == playerRow && mapCol == playerCol) {
                display = '@' | COLOR_PAIR(5);
            }
            else {
                Tile& t = world->tileAt(mapRow, mapCol);
                switch (t.terrain()) {
                    case TERRAIN::CORRIDOR:
                        display = '.';
                        break;
                    case TERRAIN::H_DOOR:
                        display = 'x';
                        break;
                    case TERRAIN::V_DOOR:
                        display = 'x';
                        break;
                    case TERRAIN::FLOOR:
                        display = '.';
                        break;
                    case TERRAIN::C_WALL:
                        display = '+';
                        break;
                    case TERRAIN::H_WALL:
                        display = ACS_HLINE;
                        break;
                    case TERRAIN::V_WALL:
                        display = ACS_VLINE;
                        break;
                    case TERRAIN::UL_WALL:
                        display = ACS_ULCORNER;
                        break;
                    case TERRAIN::UR_WALL:
                        display = ACS_URCORNER;
                        break;
                    case TERRAIN::LR_WALL:
                        display = ACS_LRCORNER;
                        break;
                    case TERRAIN::LL_WALL:
                        display = ACS_LLCORNER;
                        break;
                    case TERRAIN::TT_WALL:
                        display = ACS_TTEE;
                        break;
                    case TERRAIN::RT_WALL:
                        display = ACS_RTEE;
                        break;
                    case TERRAIN::BT_WALL:
                        display = ACS_BTEE;
                        break;
                    case TERRAIN::LT_WALL:
                        display = ACS_LTEE;
                        break;
                    default:
                        display = ' ';
                        break;
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

    KEYMAP::iterator it = _keybindings.begin();
    if ((it = _keybindings.find(c)) != _keybindings.end())
        return (game->*it->second)();

    beep();
    return STATE::ERROR;
}

void Ui::init() {
    setlocale(LC_ALL, "POSIX");
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

void Ui::setStatusWin(WINDOW *&win) {

    _status = win;
}

void Ui::setMessageWin(WINDOW *&win) {

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
