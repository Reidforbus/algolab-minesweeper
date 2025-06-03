#include "game.hpp"
#include "tui.hpp"
#include "aicore.hpp"
#include "utilities.hpp"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

void play_game(uint32_t board_height, uint32_t board_width, uint32_t mine_count){

    algolab::Minegame game;
    game.new_game(board_height, board_width, mine_count);

    algolab::GameTUI tui;
    tui.new_game(board_height, board_width);

    algolab::CSPAI ai(board_height, board_width, mine_count);


    while (!game.game_finished()){
        auto new_state = game.get_state();
        ai.update_state(new_state);
        tui.update_state(new_state);
        tui.draw();

        auto next_move = ai.get_next_move();

        if (next_move.action != algolab::FAIL){
            std::cout << "AI found a move. Press enter to play" << std::endl;
            getchar();

            if (next_move.action == algolab::OPEN){
                game.open(next_move.coord.row, next_move.coord.col);
            } else if (next_move.action == algolab::FLAG){
                game.flag(next_move.coord.row, next_move.coord.col);
            }

        } else {

            uint32_t row, col;
            char command = 'o';
            std::cin >> row >> col >> command;
            if (std::cin.fail()) {
                game.cede();
                break;
            }

            if (command == 'f') {
                game.flag(row - 1, col - 1);
            } else {
                game.open(row - 1, col - 1);
            }
        }
    }
    auto new_state = game.get_state();
    tui.update_state(new_state);
    tui.draw();
}

int main(int argc, char** argv) {

    play_game(12, 12, 40);

    return EXIT_SUCCESS;
}
