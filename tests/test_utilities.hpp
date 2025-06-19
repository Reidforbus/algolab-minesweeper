#include <catch2/catch_test_macros.hpp>
#include "../src/utilities.hpp"

namespace algolab{
    TEST_CASE( "Coord comparisons work correctly", "[Coord]" ) {
        Coord sq1(0,0);
        Coord sq2(0,1);
        Coord sq3(1,0);
        Coord sq4(1,1);
        REQUIRE(sq1 < sq2);
        REQUIRE(sq2 < sq3);
        REQUIRE(sq3 < sq4);
    }

    TEST_CASE("Coord sum works", "[Coord]") {
        Coord sq1(0,0);
        Coord sq2(0,1);
        Coord sq3(1,0);
        Coord sq4(1,1);
        sq1 += sq2;
        REQUIRE(sq1 == sq2);
        sq1 += sq3;
        REQUIRE(sq1 == sq4);
    }

    TEST_CASE("All neighbours are different", "[Coord]"){
        Coord sq(0,0);
        auto nbrs = sq.neighbours();
        for (Coord nbr : nbrs){
            sq += nbr;
        }
        REQUIRE(sq == Coord(0,0));
    }
}
