#include "../utilities.hpp"

TEST_CASE( "Coord comparisons work correctly", "[Coord]" ) {
    algolab::Coord sq1(0,0);
    algolab::Coord sq2(0,1);
    algolab::Coord sq3(1,0);
    algolab::Coord sq4(1,1);
    REQUIRE(sq1 < sq2);
    REQUIRE(sq2 < sq3);
    REQUIRE(sq3 < sq4);
}

TEST_CASE("Coord sum works", "[Coord]") {
    algolab::Coord sq1(0,0);
    algolab::Coord sq2(0,1);
    algolab::Coord sq3(1,0);
    algolab::Coord sq4(1,1);
    sq1 += sq2;
    REQUIRE(sq1 == sq2);
    sq1 += sq3;
    REQUIRE(sq1 == sq4);
}

TEST_CASE("All neighbours are different", "[Coord]"){
    algolab::Coord sq(0,0);
    auto nbrs = sq.neighbours();
    for (algolab::Coord nbr : nbrs){
        sq += nbr;
    }
    REQUIRE(sq == algolab::Coord(0,0));
}

