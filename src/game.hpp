#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <vector>

namespace algolab{
    class Minegame{
        struct Square{
            bool mine;
            bool open = false;
            bool flagged = false;
            uint32_t adjacent_mines = 0;
            uint32_t x;
            uint32_t y;
        };

        private:

        uint32_t height;
        uint32_t width;
        uint32_t mine_count;
        uint32_t opened;
        std::vector<std::vector<Square>> board;
        bool game_over;

        std::vector<std::pair<int, int>> neighbours = {{0,1},{1,1},{1,0},{0,-1},{-1,0},{-1,-1},{1,-1},{-1,1}};

        void end_game(){
            game_over = true;
            std::cout << "GAME OVER" << std::endl;
        }

        bool in_bounds(uint32_t row, uint32_t col){
            return (row >= 0 & row < height & col >= 0 & col < width);
        }

        void spread_open(uint32_t row, uint32_t col){
            uint32_t n = 0;
            std::vector<Square*> stack;
            stack.push_back(&board[row][col]);

            while (!stack.empty()){
                Square* sq = stack.back();
                stack.pop_back();
                sq->open = true;
                if (sq->adjacent_mines == 0){
                    for (auto dir : neighbours) {
                        auto j = sq->y+dir.first;
                        auto i = sq->x+dir.second;
                        if (in_bounds(j, i)){
                            Square* nbr = &board[j][i];
                            if (!nbr->open){
                                stack.push_back(nbr);
                            }
                        }
                    }
                }
            }
        }

        public:

        void new_game(uint32_t h, uint32_t w, uint32_t m){
            height = h;
            width = w;
            mine_count = m;
            game_over = false;
            board.clear();
            opened = 0;

            std::random_device rd;
            std::mt19937 randgen(rd());
            std::uniform_int_distribution<> distributor(0, (height * width) - 1);

            std::vector<bool> mines(height*width);
            for (int i = 0; i< mine_count;i++) {
                uint32_t idx = distributor(randgen);
                if (mines[idx]) {
                    i--;
                } else {
                    mines[idx] = true;
                }
            }

            for (uint32_t y = 0; y < height; y++) {
                std::vector<Square> row;

                for (uint32_t x = 0; x < width; x++) {
                    Square next;
                    next.x = x;
                    next.y = y;
                    next.mine = mines[(y*width) + x];
                    row.push_back(next);
                }
                board.push_back(row);
            }

            for (uint32_t y = 0; y < height; y++) {
                for (uint32_t x = 0; x < width; x++) {
                    Square* slot = &board[y][x];
                    if (slot->mine) {
                        for (auto dir : neighbours) {
                            auto j = y+dir.first;
                            auto i = x+dir.second;
                            if (in_bounds(j, i)){
                                board[j][i].adjacent_mines++;
                            }
                        }
                    }
                }
            }
        }



        bool open(uint32_t row, uint32_t col) {
            if (!in_bounds(row, col)) {
                std::cout << "Was out of bounds" << std::endl;
                return true;
            }

            Square* slot = &board[row][col];
            if (slot->open){
                return true;
            }

            if (slot->mine){
                end_game();
            } else {
                opened = spread_open(row, col);
            }

            return !slot->mine;
        }

        std::vector<uint32_t> get_state() {
            std::vector<uint32_t> state;
            for (auto row : board) {
                for (Square sq : row) {
                    uint32_t x = 10;
                    if (sq.flagged) {
                        x = 11;
                    } else if (sq.open & sq.mine) {
                        x = 9;
                    } else if (sq.open) {
                        x = sq.adjacent_mines;
                    }
                    state.push_back(x);
                }
            }
            return state;
        }

        void flag(uint32_t row, uint32_t col) {
            Square* slot = &board[row][col];
            if (slot->open){
                return;
            }
            slot->flagged = !slot->flagged;
        }

        void print_state() {
            for (auto i : board) {
                for (Square j : i) {
                    if (j.open & j.mine) {
                        std::cout << "@";
                    } else if (j.open & (j.adjacent_mines == 0)) {
                        std::cout << " ";
                    } else if (j.open) {
                        std::cout << j.adjacent_mines;
                    } else {
                        std::cout << "#";
                    }
                }
                std::cout << std::endl;
            }
        }
    };
} //namespace algolab
