#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include "minesweeper/aicore.hpp"

namespace algolab{
    class CSPAITest{
        CSPAI& ai;

        public:
        CSPAITest(CSPAI& cspai):
            ai(cspai){}

        Move make_guess(){
            ai.make_guess();
            return ai.get_next_move();
        }

        void clear_changed(){
            ai.changed.clear();
        }

        int size_of_changed(){
            return ai.changed.size();
        }

        bool in_bounds(int row, int col){
            return ai.in_bounds({row, col});
        }

        bool calc_moves(){
            return ai.calculate_moves();
        }
    };

    bool contains_move(const std::vector<Move>& moves, Move move){
        return *std::find(moves.begin(), moves.end(), move) == move;
    }

    TEST_CASE("Update state only updates changed squares", "[aicore]"){
        CSPAI ai(3,3,1);
        CSPAITest tester(ai);

        ai.update_state({
                10,10,10,
                10, 7,10,
                10,10,10
                });
        REQUIRE(tester.size_of_changed() == 1);
        tester.clear_changed();
        ai.update_state({
                10,10,10,
                10, 7, 4,
                10,10,10
                });
        REQUIRE(tester.size_of_changed() == 1);
    }

    TEST_CASE("First four guesses are in the corners", "[aicore]"){
        CSPAI ai(3,3,1);
        CSPAITest tester(ai);

        ai.update_state({
                10,10,10,
                10,10,10,
                10,10,10
                });
        for (int i = 0; i < 4; ++i){
            auto [row, col] = tester.make_guess().coord;
            bool in_corner = (row == 0 || row == 2) && (col == 0 || col == 2);
            REQUIRE(in_corner);
        }
    }

    // Since the guessing is random
    TEST_CASE("After corners are open, guesses are random (RERUN IF FAILS!)", "[aicore]"){
        CSPAI ai(9,9,1);
        CSPAITest tester(ai);

        ai.update_state({
                2 ,10,10,10,10,10,10,10, 2,
                10,10,10,10,10,10,10,10,10,
                10,10,10,10,10,10,10,10,10,
                10,10,10,10,10,10,10,10,10,
                10,10,10,10,10,10,10,10,10,
                10,10,10,10,10,10,10,10,10,
                10,10,10,10,10,10,10,10,10,
                10,10,10,10,10,10,10,10,10,
                2 ,10,10,10,10,10,10,10, 2
                });
        auto [row, col] = tester.make_guess().coord;
        bool in_corner = (row == 0 || row == 8) && (col == 0 || col == 8);
        REQUIRE_FALSE(in_corner);
        auto [row2, col2] = tester.make_guess().coord;
        in_corner = (row2 == 0 || row2 == 8) && (col2 == 0 || col2 == 8);
        REQUIRE_FALSE(in_corner);
        auto [row3, col3] = tester.make_guess().coord;
        in_corner = (row3 == 0 || row3 == 8) && (col3 == 0 || col3 == 8);
        REQUIRE_FALSE(in_corner);

        bool all_same = (row == row2 && row == row3) && (col == col2 && col == col3);
        REQUIRE_FALSE(all_same);
    }

    TEST_CASE("in_bounds works correctly", "[aicore]"){
        CSPAI ai(3,3,1);
        CSPAITest tester(ai);

        CHECK(tester.in_bounds(0,0));
        CHECK_FALSE(tester.in_bounds(-1,0));
        CHECK_FALSE(tester.in_bounds(0,-1));
        CHECK_FALSE(tester.in_bounds(-1,-1));
        CHECK(tester.in_bounds(2,2));
        CHECK(tester.in_bounds(1,2));
        CHECK(tester.in_bounds(2,1));
        CHECK_FALSE(tester.in_bounds(3,2));
        CHECK_FALSE(tester.in_bounds(2,3));
        CHECK_FALSE(tester.in_bounds(3,3));
    }

    TEST_CASE("AI finds trivial mine", "[aicore]"){
        CSPAI ai(3,3,1);
        CSPAITest tester(ai);

        ai.update_state({
                1 ,1 ,1 ,
                1 ,10,1 ,
                1 ,1 ,1 
                });

        auto move = ai.get_next_move();
        REQUIRE(move.action == FLAG);
        REQUIRE(move.coord == Coord{1,1});
    }

    TEST_CASE("AI finds trivial open square", "[aicore]"){
        CSPAI ai(3,3,0);
        CSPAITest tester(ai);

        ai.update_state({
                 0, 0, 0,
                 0,10, 0,
                 0, 0, 0 
                });

        auto move = ai.get_next_move();
        REQUIRE(move.action == OPEN);
        REQUIRE(move.coord == Coord{1,1});
    }

    TEST_CASE("AI cannot find solution when there isn't enough information", "[aicore]"){
        CSPAI ai(4,4,64);
        CSPAITest tester(ai);

        ai.update_state({
                 1, 2,10, 1,
                10,10, 2, 1,
                10,10, 2, 1,
                 1, 2,10, 1,
                });

        ai.get_next_move();
        ai.get_next_move();
        auto move = ai.get_next_move();
        REQUIRE(move.guess);
    }

    TEST_CASE("AI return FAIL move if board is open", "[aicore]"){
        CSPAI ai(3,3,0);
        CSPAITest tester(ai);

        ai.update_state({
                 0, 0, 0,
                 0, 0, 0,
                 0, 0, 0 
                });

        auto move = ai.get_next_move();
        REQUIRE(move.action == FAIL);
    }

    TEST_CASE("AI finds solutions when it requires brute force", "[aicore]"){
        CSPAI ai(4,4,4);
        CSPAITest tester(ai);

        ai.update_state({
                10,10, 1, 0,
                10, 2, 1, 0,
                10, 3, 1, 0,
                10,10, 1, 0
                });

        std::vector<Move> moves;
        for (int i = 0; i < 5; i++){
            moves.push_back(ai.get_next_move());
        }

        REQUIRE(contains_move(moves, Move{{0,1}, FLAG, false}));
        REQUIRE(contains_move(moves, Move{{3,1}, FLAG, false}));
        REQUIRE(contains_move(moves, Move{{0,0}, OPEN, false}));
        REQUIRE(contains_move(moves, Move{{3,0}, FLAG, false}));
    }
} //namespace algolab
