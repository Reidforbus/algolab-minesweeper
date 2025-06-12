#pragma once

#include "utilities.hpp"
#include "csp.hpp"
#include <random>

namespace algolab{
    class CSPAI{
        private:
            int height, width, mine_count;
            std::map<Coord, int> opened;
            std::set<Coord> mines;
            std::set<Coord> unknown;
            std::vector<Coord> changed;
            std::vector<Coord> to_open;
            std::vector<Coord> to_flag;
            CSPGraph<Coord> csp;
            int end_game_threshold = 16;
            bool end_game = false;

            bool in_bounds(const Coord& sq){
                return (sq.row >= 0) && (sq.row < height) && (sq.col >= 0) && (sq.col < width);
            }

            bool check_end_game(){
                if (!end_game){
                    if (unknown.size() < end_game_threshold){
                        std::cerr << "Entering end game" << std::endl;
                        end_game = true;
                        std::vector<Coord> variables(unknown.begin(), unknown.end());
                        csp.add_constraint(variables, mine_count - mines.size());
                        return true;
                    }
                }
                return false;
            }

            bool calculate_moves(){
                bool progressed = check_end_game();

                while (!changed.empty()){
                    Coord open_sq = changed.back();
                    changed.pop_back();

                    int value = opened[open_sq];
                    csp.update_remaining_ones(mine_count - mines.size());
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
                        mines.insert(sq);
                        unknown.erase(sq);
                        to_flag.push_back(sq);
                    }

                    progressed = true;
                }

                return progressed;
            }

            bool make_guess(){
                const Coord corners[] = {{0, 0}, {0, int(width) - 1}, {int(height) - 1, 0}, {int(height) - 1, int(width) - 1}};
                for (Coord sq : corners){
                    if (unknown.count(sq)){
                        to_open.push_back(sq);
                        return true;
                    }
                }

                if (!unknown.empty()){
                    std::uniform_int_distribution<> distributor(0, unknown.size()-1);
                    std::random_device rd;
                    std::mt19937 rng(rd());
                    uint32_t idx = distributor(rng);
                    auto iter = unknown.cbegin();
                    for (int i = 0; i < idx; i++){
                        iter++;
                    }
                    to_open.push_back(*iter);
                    return true;
                }

                return false;
            }

        public:
            CSPAI(int h, int w, int m):
                height(h), width(w), mine_count(m){
                        for (auto j = 0;j < height; j++){
                            for (auto i = 0;i < width; i++){
                                Coord sq(j, i);
                                unknown.insert(sq);
                            }
                        }
                }

            void update_state(std::vector<int> state){
                for (auto j = 0;j < height; j++){
                    for (auto i = 0;i < width; i++){
                        Coord sq(j, i);
                        if (opened.count(sq)){
                            continue;
                        }
                        auto val = state[(j * width) + i] % 16;
                        if (val < 9){
                            changed.push_back(sq);
                            opened[sq] = val;
                            unknown.erase(sq);
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
                        if (csp.brute_force_search()){
                            continue;
                        }
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
