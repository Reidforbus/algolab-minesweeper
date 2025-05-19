#include "game.hpp"
#include "tui.hpp"
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

int main(int argc, char** argv) {

    Minegame game;
    game.new_game(9,9,20);

    GameTUI tui;
    tui.new_game(9,9);

    tui.draw();

    while (true){
        uint32_t row, col;
        std::cin >> col >> row;
        std::cout << col << " " << row << std::endl;

        game.open(row, col);
        game.print_state();
    }

    return EXIT_SUCCESS;
}
