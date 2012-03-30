// Game --The controller class for AitDoB (implementation)

#include <cstdlib>
#include <ctime>
using namespace std;

#include "door.h"
#include "game.h"
#include "player.h"
#include "tile.h"
#include "ui.h"
#include "world.h"

int Game::run() {
    srand(time(NULL));
    STATE phase = STATE::COMMAND;
    World world;
    world.create();
    Ui ui;
    ui.init();
    ui.message("Ascent into the Depths of Beyond");
    while(1) {
        switch (phase) {
            case STATE::COMMAND:
                phase = ui.handleInput();
                break;
            case STATE::DOOR:
                phase = door();
                break;
            case STATE::QUIT:
                goto end;
            case STATE::ERROR:
            default:
                phase = error();
        }
        ui.draw();
    }

end:
    ui.message("--press a key to continue--");
    ui.pause();
    ui.end();

    return EXIT_SUCCESS;
}

STATE Game::door() {
    Ui ui;
    World world;
    Door& d = dynamic_cast<Door&>
       (world.tileAt(world.playerRow(), world.playerCol()).contents());
    if (d.open() == true)
        return STATE::COMMAND;

    Player& p = world.player();

    while (1) {
        int damage = d.attack();
        damage -= p.defend();
        if ( damage > 0 ) {
            p.setHealth(-damage);
            ui.message("The door does not budge.");
        }
        if ( p.health() < 1 ) {
            ui.message("You have died trying to break down the door");
            return STATE::QUIT;
        }

        damage = p.attack();
        damage -= d.defend();
        if ( damage > 0 ) {
            d.setHealth(-damage);
            ui.message("The door shudders");
        }

        if ( d.health() < 1 ) {
            ui.message("You have broken down the door.");
            world.tileAt( world.playerRow(),
                world.playerCol() ).setTerrain(TERRAIN::FLOOR);
            break;
        }

    }
    return STATE::COMMAND;
}

STATE Game::error() {
    Ui ui;
    ui.message("Huh?");
    return STATE::COMMAND;
}

STATE Game::move_left() {
    return move( 0, -1 );
}

STATE Game::move_down() {
    return move( 1, 0 );
}

STATE Game::move_up() {
    return move( -1, 0 );
}

STATE Game::move_right() {
    return move( 0, 1 );
}

STATE Game::move_upleft() {
    return move( -1, -1 );
}

STATE Game::move_upright() {
    return move( -1, 1 );
}
STATE Game::move_downleft() {
    return move( 1, -1 );
}

STATE Game::move_downright() {
    return move( 1, 1 );
}

STATE Game::move(int row, int col) {
    Ui ui;
    World world;
    int temprow = world.playerRow() + row;
    int tempcol = world.playerCol() + col;
    Tile& t = world.tileAt( temprow, tempcol );
    if (temprow > -1
        && temprow < world.height()
        && tempcol > -1
        && tempcol < world.width()
        && t.passable())
    {
        world.setPlayerRow(temprow);
        world.setPlayerCol(tempcol);
        Ui ui;
        ui.message("");
        if (t.terrain() == TERRAIN::H_DOOR || t.terrain() == TERRAIN::V_DOOR) {
            return STATE::DOOR;
        }
    }
    else {
        ui.message("You can't go there!");
    }
    return STATE::COMMAND;
}

STATE Game::quit() {
    return STATE::QUIT;
}

STATE Game::refresh() {
    Ui ui;
    ui.refresh();

    return STATE::COMMAND;
}

STATE Game::resize() {
    Ui ui;
    ui.resize();

    return STATE::COMMAND;
}

STATE Game::shell() {
    Ui ui;
    ui.shell();

    return STATE::COMMAND;
}
