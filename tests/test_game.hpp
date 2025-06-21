#include <catch2/catch_test_macros.hpp>
#include "minesweeper/game.hpp"

namespace algolab{
    class MinegameTest {
        Minegame& game;

        public:
        MinegameTest(Minegame& g): game(g){}

        void test_in_bounds(){
            game.new_game(10, 10, 1, 0, true);

            REQUIRE(game.in_bounds(0, 0));
            REQUIRE(game.in_bounds(9, 9));

            REQUIRE_FALSE(game.in_bounds(-1,-1));
            REQUIRE_FALSE(game.in_bounds(0,-1));
            REQUIRE_FALSE(game.in_bounds(-1,0));

            REQUIRE_FALSE(game.in_bounds(0,10));
            REQUIRE_FALSE(game.in_bounds(10,0));
            REQUIRE_FALSE(game.in_bounds(10,10));
            REQUIRE_FALSE(game.in_bounds(-1,10));
            REQUIRE_FALSE(game.in_bounds(10,-1));

        }

        void test_number_of_mines(int n){
            game.new_game(10, 10, n, 0, true);
            REQUIRE(count_mines() == n);
        }

        int count_mines(){
            int n = 0;
            for (auto row : game.board){
                for (auto sq : row){
                    if (sq.mine){
                        n++;
                    }
                }
            }
            return n;
        }

        void check_generation(uint32_t seed, int n){
            game.new_game(5,5,n,seed,true);
            REQUIRE(count_mines() == n);

            for (int i = 0; i < 25; i++){
                bool mine = game.board[i/5][i%5].mine;
                if (i < n){
                    REQUIRE(mine);
                } else {
                    REQUIRE_FALSE(mine);
                }
            }
        }

        Coord get_last_move(){
            return game.last_move;
        }

        auto get_square(int row, int col){
            return game.board.at(row).at(col);
        }

        uint32_t find_pattern_seed(std::string s){
            uint32_t test_seed = 0;
            int n = 0;
            for (char c : s){
                n += (c == '@');
            }
            std::cout << "Found " << n << " mines" << std::endl;
            while (true){
                game.new_game(5,5,n,test_seed++ ,true);
                bool valid = true;
                for (int i = 0; i < 25; i++){
                    bool mine = game.board[i/5][i%5].mine;
                    if (mine != (s[i] == '@')) valid = false;
                }
                if (valid){
                    game.print_state();
                    return game.get_seed();
                }
            }
        }
    };

    TEST_CASE("Game can be generated with valid parameters", "[Game]"){
        Minegame game;
        REQUIRE_NOTHROW(game.new_game(10, 10, 1, 0, true));
        REQUIRE_NOTHROW(game.new_game(10, 10, 99, 0, true));
    }

    TEST_CASE("Game cannot be generated with faulty parameters", "[Game]"){
        Minegame game;
        REQUIRE_THROWS(game.new_game(10, 10, 101, 0, true));
        REQUIRE_THROWS(game.new_game(10, 10, 100, 0, true));
        REQUIRE_THROWS(game.new_game(10, 10, 0, 0, true));
        REQUIRE_THROWS(game.new_game(10, 10, -1, 0, true));

        REQUIRE_THROWS(game.new_game(-1, 10, 5, 0, true));
        REQUIRE_THROWS(game.new_game(0, 10, 5, 0, true));


        REQUIRE_THROWS(game.new_game(10, -10, 5, 0, true));
        REQUIRE_THROWS(game.new_game(10, 0, 5, 0, true));
    }

    TEST_CASE("in_bounds works correctly", "[Game]"){
        Minegame game;
        MinegameTest(game).test_in_bounds();
    }

    TEST_CASE("Correct number of mines on board", "[Game]"){
        Minegame game;
        MinegameTest(game).test_number_of_mines(1);
        MinegameTest(game).test_number_of_mines(50);
        MinegameTest(game).test_number_of_mines(99);
    }

    TEST_CASE("Seeded board generation works expectedly", "[Game]"){
        Minegame game;
        MinegameTest gametester(game);

        gametester.check_generation(9, 1);
        gametester.check_generation(729, 2);
        gametester.check_generation(1736, 3);
        gametester.check_generation(14763, 4);
        gametester.check_generation(100311, 5);
        gametester.check_generation(207577, 6);
        gametester.check_generation(148667, 7);
        gametester.check_generation(106828, 8);
        gametester.check_generation(973441, 9);
        gametester.check_generation(957779, 10);
        gametester.check_generation(10660902, 11);
        gametester.check_generation(6336529, 12);
        gametester.check_generation(1200796, 13);
        gametester.check_generation(3426934, 14);
        gametester.check_generation(4998722, 15);
        gametester.check_generation(118842, 16);
        gametester.check_generation(2210161, 17);
        gametester.check_generation(1052207, 18);
        gametester.check_generation(35525, 19);
        gametester.check_generation(128201, 20);
        gametester.check_generation(26072, 21);
        gametester.check_generation(1201, 22);
        gametester.check_generation(69, 23);
        gametester.check_generation(10, 24);

    }

    TEST_CASE("Game is over and lost when mined square is opened", "[Game]"){
        Minegame game;
        game.new_game(5,5,1,9,true);
        game.open(0,0);
        REQUIRE(game.game_finished());
        REQUIRE_FALSE(game.was_won());
    }

    TEST_CASE("Game is not over when free square is opened", "[Game]"){
        Minegame game;
        game.new_game(5,5,6,30627,true);
        game.open(4,4);
        REQUIRE_FALSE(game.game_finished());
        game.open(0,0);
        REQUIRE(game.game_finished());
    }

    TEST_CASE("Game is over and won when no more unopened free squares remain", "[Game]"){
        Minegame game;
        game.new_game(5,5,5,100311,true);
        game.open(4,4);
        REQUIRE(game.game_finished());
        REQUIRE(game.was_won());
    }

    TEST_CASE("Game is over and not won when game has ceded", "[Game]"){
        Minegame game;
        game.new_game(5,5,5,0,false);
        REQUIRE_FALSE(game.game_finished());
        game.cede();
        REQUIRE(game.game_finished());
        REQUIRE_FALSE(game.was_won());
    }

    TEST_CASE("Opening is aborted if Coord is out of bounds", "[Game]"){
        Minegame game;
        MinegameTest tester(game);
        game.new_game(5,5,6,30627,true);
        game.open(4,4);
        REQUIRE(tester.get_last_move() == Coord(4,4));
        game.open(-1,-1);
        REQUIRE(tester.get_last_move() == Coord(4,4));
    }

    TEST_CASE("Opening is aborted if Coord is already open", "[Game]"){
        Minegame game;
        MinegameTest tester(game);
        game.new_game(5,5,6,30627,true);
        game.open(4,4);
        REQUIRE(tester.get_last_move() == Coord(4,4));
        game.open(3,4);
        REQUIRE(tester.get_last_move() == Coord(4,4));
    }

    TEST_CASE("Flagging outside bounds does nothing", "[Game]"){
        Minegame game;
        MinegameTest tester(game);
        game.new_game(5,5,6,30627,true);
        game.flag(-2,-2);
        REQUIRE(tester.get_last_move() == Coord(-1,-1));
    }

    TEST_CASE("Flagging open square does nothing", "[Game]"){
        Minegame game;
        MinegameTest tester(game);
        game.new_game(5,5,6,30627,true);
        game.open(4,4);
        REQUIRE(tester.get_last_move() == Coord(4,4));
        game.flag(3,4);
        REQUIRE(tester.get_last_move() == Coord(4,4));
    }

    TEST_CASE("Flagging square changes square state", "[Game]"){
        Minegame game;
        MinegameTest tester(game);
        game.new_game(5,5,6,30627,true);
        game.flag(4,4);
        auto sq = tester.get_square(4, 4);
        REQUIRE(sq.flagged);
    }

    TEST_CASE("Flagging flagged square removes flag", "[Game]"){
        Minegame game;
        MinegameTest tester(game);
        game.new_game(5,5,6,30627,true);
        game.flag(4,4);
        game.flag(4,4);
        auto sq = tester.get_square(4, 4);
        REQUIRE_FALSE(sq.flagged);
    }

    TEST_CASE("State is returned correctly", "[Game]"){
        Minegame game;
        MinegameTest tester(game);
        game.new_game(5,5,6,30627,true);
        game.open(4,4);
        game.flag(0,0);
        std::vector<int> correct_values = {27, 10, 10, 2, 0, 10, 10, 10, 3, 0, 10, 10, 10, 3, 0, 10, 10, 10, 3, 0, 10, 10, 10, 2, 0};
        REQUIRE(correct_values == game.get_state());
        game.open(0,2);
        correct_values = {9, 3, 25, 2, 0, 1, 4, 9, 3, 0, 0, 3, 9, 3, 0, 0, 3, 9, 3, 0, 0, 2, 9, 2, 0};
        REQUIRE(correct_values == game.get_state());
    }
}
