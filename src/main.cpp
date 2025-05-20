#include "game.hpp"
#include "tui.hpp"
#include "cspsolver.hpp"
#include <cstdint>
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

void play_game(uint32_t board_height, uint32_t board_width, uint32_t mine_count){

    algolab::Minegame game;
    game.new_game(board_height, board_width, mine_count);

    algolab::GameTUI tui;
    tui.new_game(board_height, board_width);

    algolab::CSPAI ai(board_height, board_width, mine_count);


    while (true){
        auto new_state = game.get_state();
        tui.update_state(new_state);
        ai.update_state(new_state);
        tui.draw();

        uint32_t row, col;
        char command = 'o';
        std::cin >> row >> col >> command;
        if (std::cin.fail()) {
            break;
        }

        if (command == 'f') {
            game.flag(row - 1, col - 1);
        } else {
            game.open(row - 1, col - 1);
        }
    }
}

int main(int argc, char** argv) {

    play_game(12, 12, 40);

    return EXIT_SUCCESS;
}
