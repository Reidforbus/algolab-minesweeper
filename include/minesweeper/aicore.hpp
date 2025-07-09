#pragma once

#include "utilities.hpp"
#include "csp.hpp"
#include <queue>

namespace algolab{
    class CSPAI{
        friend class CSPAITest;
        int height, width, mine_count;
        std::map<Coord, int> opened;
        std::set<Coord> mines;
        std::set<Coord> unknown;
        std::vector<Coord> changed;
        std::queue<Coord> to_open;
        std::queue<Coord> to_flag;
        CSPGraph<Coord> csp;

        bool in_bounds(const Coord& sq);

        bool calculate_moves();

        bool make_guess();

        public:
        CSPAI(int h, int w, int m);

        void update_state(std::vector<int> state);

        Move get_next_move();


    };
} // namespace algolab
