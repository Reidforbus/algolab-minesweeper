#include "game.hpp"
#include "tui.hpp"
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main(int argc, char** argv) {

    algolab::Minegame game;
    game.new_game(9,9,20);

    algolab::GameTUI tui;
    tui.new_game(9,9);

    tui.draw();

    while (true){
        uint32_t row, col;
        std::cin >> col >> row;
        if (std::cin.fail()) {
            break;
        }

        game.open(row, col);
        auto new_state = game.get_state();
        tui.update_state(new_state);
        tui.draw();
    }

    return EXIT_SUCCESS;
}
