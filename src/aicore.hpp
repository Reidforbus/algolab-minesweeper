#pragma once

#include "utilities.hpp"
#include "csp.hpp"
#include <cstdint>
#include <vector>

namespace algolab{
    class CSPAI{
        private:
            uint32_t height, width, mine_count;
            std::map<Coord, uint32_t> opened;
            std::vector<Coord> changed;
            std::vector<Coord> to_open;
            std::vector<Coord> to_flag;
            CSPGraph<Coord> csp;

            bool in_bounds(const Coord& sq){
                return (sq.row >= 0) && (sq.row < height) && (sq.col >= 0) && (sq.col < width);
            }

            bool calculate_moves(){
                bool progressed = false;

                while (!changed.empty()){
                    Coord open_sq = changed.back();
                    changed.pop_back();

                    uint32_t value = opened[open_sq];
                    if (value != 0) {

                        std::vector<Coord> nbrs = open_sq.neighbours();
                        std::vector<Coord> variables;

                        for (Coord sq : nbrs){
                            if (in_bounds(sq) && (opened.count(sq) == 0)){
                                variables.push_back(sq);
                            }
                        }
                        csp.add_constraint(variables, value);
                    }
                    csp.add_opened(open_sq);
                }

                while (true){
                    auto [sq, value] = csp.get_solution();
                    if (value < 0){
                        break;
                    } else if (value == 0) {
                        to_open.push_back(sq);
                    } else if (value == 1) {
                        to_flag.push_back(sq);
                    }

                    progressed = true;
                }

                return progressed;
            }

            bool make_guess(){
                const Coord corners[] = {{0, 0}, {0, int(width) - 1}, {int(height) - 1, 0}, {int(height) - 1, int(width) - 1}};
                for (Coord sq : corners){
                    if (opened.count(sq) == 0){
                        to_open.push_back(sq);
                        return true;
                    }
                }
                return false;
            }

        public:
            CSPAI(uint32_t h, uint32_t w, uint32_t m):
                height(h), width(w), mine_count(m){}

            void update_state(std::vector<uint32_t> state){
                for (auto j = 0;j < height; j++){
                    for (auto i = 0;i < width; i++){
                        Coord sq = {j, i};
                        if (opened.count(sq)){
                            continue;
                        }
                        auto val = state[(j * width) + i] % 16;
                        if (val < 9){
                            changed.push_back(sq);
                            opened[sq] = val;
                        }
                    }
                }
            }

            Move get_next_move(){

                while (true) {
                    if (!to_flag.empty()){
                        Move next = {to_flag.back(), FLAG, false};
                        to_flag.pop_back();
                        return next;
                    }

                    while (!to_open.empty()){
                        Move next = {to_open.back(), OPEN, false};
                        to_open.pop_back();
                        if (opened.count(next.coord) > 0) continue;
                        return next;
                    }

                    bool found_moves = calculate_moves();

                    if (!found_moves){
                        if (make_guess()){
                            Move guess = {to_open.back(), OPEN, true};
                            to_open.pop_back();
                            return guess;
                        }

                        break;
                    }

                }
                return Move{Coord(), FAIL, false};
            }


    };
} // namespace algolab
