// aitdob.cc -- Ascent into the Depths of Beyond (main file)

using namespace std;

#include "game.h"
#include "ui.h"
#include "world.h"

Game*     game = Game::instance();
Ui*       ui = Ui::instance();
World*    world = World::instance();

int main (int, char **) {
    return game->run();
}
