// aitdob.cc -- Ascent into the Depths of Beyond (main file)

#include <memory>
using namespace std;

#include "game.h"
#include "ui.h"
#include "world.h"


int main (int, char **) {
    Game game;
    return game.run();
}
