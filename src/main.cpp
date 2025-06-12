#include "game.hpp"
#include "tui.hpp"
#include "aicore.hpp"
#include "utilities.hpp"
#include <limits>

#define BOARD_HEIGHT 16
#define BOARD_WIDTH 30
#define NUMBER_OF_MINES 99

void play_game(int board_height, int board_width, int mine_count, uint32_t seed, bool seeded){

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
            if (next_move.guess){
            std::cout << "AI is making a guess. Press enter to play" << std::endl;
            } else {
            std::cout << "AI found a move. Press enter to play" << std::endl;
            }
            getchar();

            if (next_move.action == algolab::OPEN){
                game.open(next_move.coord.row, next_move.coord.col);
            } else if (next_move.action == algolab::FLAG){
                game.flag(next_move.coord.row, next_move.coord.col);
            }

        } else {

            std::cerr << "AI couldn't make a move, can you help?" << std::endl;
            int row, col;
            char command = 'x';
            while (true) {
                std::cin >> command;
                if (command == 'q') {
                    game.cede();
                    break;
                }
                std::cin >> row >> col;
                if (std::cin.fail()) {
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
                seed = std::stoul(std::string(argv[i]));
                seeded = true;
            } catch (std::invalid_argument) {
                std::cerr << "Invalid seed. Please give a whole number" << std::endl;
                return EXIT_FAILURE;
            } catch (std::out_of_range) {
                std::cerr << "Seed is too big. It can't be more than " << ((uint32_t)0 - 1) << std::endl;
                return EXIT_FAILURE;
            }

        }
    }


    play_game(BOARD_HEIGHT, BOARD_WIDTH, NUMBER_OF_MINES, seed, seeded);

    return EXIT_SUCCESS;
}
