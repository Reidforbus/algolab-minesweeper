#pragma once

#include "utilities.hpp"
#include <iostream>

namespace algolab{
    class Minegame{
        friend class MinegameTest;

        struct Square{
            bool mine;
            bool open = false;
            bool flagged = false;
            int adjacent_mines = 0;
            Coord coord;
        };

        int height;
        int width;
        int mine_count;
        std::vector<std::vector<Square>> board;
        bool game_over;
        bool winner;
        Coord last_move;
        uint32_t seed;


        void game_lost();
        void game_won();
        bool check_board();
        bool in_bounds(int row, int col);
        void spread_open(int row, int col);
        void fill_numbers();
        void fill_board(std::vector<bool> mines);

        public:

        Minegame();
        bool game_finished();
        bool was_won();
        void cede();
        uint32_t get_seed();

        void new_game(int h, int w, int m, uint32_t new_seed, bool seeded);
        void from_coords(int h, int w, std::vector<Coord> mines);
        void from_string(int h, int w, std::string input);
        void open(int row, int col);
        void flag(int row, int col);
        bool execute_move(algolab::Move move);
        std::vector<int> get_state();
        void print_state(std::ostream& output = std::cout);
    };
} //namespace algolab
