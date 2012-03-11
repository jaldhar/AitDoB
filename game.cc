// Game --The controller class for AitDoB (implementation)

#include <cstdlib>
#include <ctime>
using namespace std;

#include "game.h"
#include "ui.h"

Game* Game::_instance = 0;

extern Ui* ui;

Game* Game::instance() {
    if(_instance == 0)
        _instance = new Game();

    return _instance;
}

Game::Game() {
}

Game::~Game() {
    if(_instance != 0)
        delete _instance;
}

int Game::run() {
    srand(time(NULL));
    STATE phase = COMMAND;
    ui->init();

    while(1) {
        switch (phase) {
            case COMMAND:
                phase = ui->handleInput();
                break;
            case QUIT:
                goto end;
            case ERROR:
            default:
                phase = error();
        }
        ui->draw();
    }

end:
    ui->pause();
    ui->end();

    return EXIT_SUCCESS;
}

STATE Game::error() {

    return COMMAND;
}

STATE Game::quit() {
    return QUIT;
}

STATE Game::refresh() {
    ui->refresh();

    return COMMAND;
}

STATE Game::resize() {
    ui->resize();

    return COMMAND;
}

STATE Game::shell() {
    ui->shell();

    return COMMAND;
}
