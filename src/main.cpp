#include "game.hpp"
#include "tui.hpp"
#include "aicore.hpp"
#include "utilities.hpp"
#include <bits/types/error_t.h>
#include <cstdint>
#include <cstdio>
#include <ios>
#include <iostream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <limits>
#include <stdexcept>

#define BOARD_WIDTH 9
#define BOARD_HEIGHT 9
#define NUMBER_OF_MINES 10

void play_game(uint32_t board_height, uint32_t board_width, uint32_t mine_count, int seed, bool seeded){

    algolab::Minegame game;
    game.new_game(board_height, board_width, mine_count, seed, seeded);

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
            char command = 'x';
            while (true) {
                std::cin >> command >> row >> col;
                if (std::cin.fail()) {
                    if (command == 'q') {
                        game.cede();
                    }
                    std::cout << "Could not parse command. Try again." << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (command == 'f') {
                    game.flag(row - 1, col - 1);
                } else if (command == 'o') {
                    game.open(row - 1, col - 1);
                } else if (command == 'q'){
                    game.cede();
                }

                break;
            }
        }
    }
    auto new_state = game.get_state();
    tui.update_state(new_state);
    tui.draw();
}

int main(int argc, char** argv) {

    int i = 1;
    int seed = 0;
    bool seeded = false;
    if (argc >= 3) {
        std::string flag(argv[i++]);
        if (flag.compare("-s") == 0){
            try {
                seed = stoi(std::string(argv[i]));
                seeded = true;
            } catch (std::invalid_argument) {
                std::cerr << "Invalid seed. Please give a whole number" << std::endl;
                return EXIT_FAILURE;
            }

        }
    }


    play_game(BOARD_HEIGHT, BOARD_WIDTH, NUMBER_OF_MINES, seed, seeded);

    return EXIT_SUCCESS;
}
