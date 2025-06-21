#include <catch2/catch_test_macros.hpp>
#include "minesweeper/csp.hpp"


namespace algolab {
    class CSPGraphTest{
        CSPGraph<char>& csp;

        public:

        CSPGraphTest(CSPGraph<char>& c): csp(c){}

        bool find_matching_constraint(std::vector<char> vars, int sum){
            for (auto&& c : csp.constraints){
                if (c->sum != sum) continue;
                bool hit = true;
                for (char var : vars){
                    auto& var_object = csp.variables[var];
                    if (c->unknown.count(&var_object) == 0){
                        hit = false;
                        break;
                    }
                }
                if (hit){
                    return true;
                }
            }
            return false;
        }

        int get_number_of_active_constraints(){
            int result = 0;
            for (auto&& c : csp.constraints){
                if (c->obsolete()) continue;
                result++;
            }
            return result;
        }

        int get_number_of_constraints(){
            return csp.constraints.size();
        }

        bool update_stack_empty(){
            return csp.update_stack.empty();
        }

    };

    TEST_CASE("Algorithm solves trivial all-ones constraint" , "[CSP]" ){
        CSPGraph<char> csp;
        csp.add_constraint({'A', 'B', 'C'}, 3);
        auto result = csp.get_solution();
        REQUIRE(result.second == 1);
        result = csp.get_solution();
        REQUIRE(result.second == 1);
        result = csp.get_solution();
        REQUIRE(result.second == 1);
        result = csp.get_solution();
        REQUIRE(result.second == -1);
    }

    TEST_CASE("Algorithm solves trivial all-zeroes constraint" , "[CSP]" ){
        CSPGraph<char> csp;
        csp.add_constraint({'A', 'B', 'C'}, 0);
        auto result = csp.get_solution();
        REQUIRE(result.second == 0);
        result = csp.get_solution();
        REQUIRE(result.second == 0);
        result = csp.get_solution();
        REQUIRE(result.second == 0);
        result = csp.get_solution();
        REQUIRE(result.second == -1);
    }

    TEST_CASE("Algorithm simplifies constraint correctly when superset given first", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);
        csp.add_constraint({'A', 'B', 'C', 'D'}, 2);
        auto result = csp.get_solution();
        REQUIRE(result.second == -1);
        REQUIRE(tester.update_stack_empty());
        csp.add_constraint({'A', 'B'}, 1);
        result = csp.get_solution();
        REQUIRE(result.second == -1);

        REQUIRE(tester.get_number_of_active_constraints() == 2);
        REQUIRE(tester.find_matching_constraint({'A', 'B'}, 1));
        REQUIRE(tester.find_matching_constraint({'C', 'D'}, 1));

        REQUIRE_FALSE(tester.find_matching_constraint({'A', 'B', 'C', 'D'}, 2));
    }

    TEST_CASE("Algorithm simplifies constraint correctly when subset given first", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);
        csp.add_constraint({'A', 'B'}, 1);
        auto result = csp.get_solution();
        REQUIRE(result.second == -1);
        REQUIRE(tester.update_stack_empty());
        csp.add_constraint({'A', 'B', 'C', 'D'}, 2);
        result = csp.get_solution();
        REQUIRE(result.second == -1);

        REQUIRE(tester.get_number_of_active_constraints() == 2);
        REQUIRE(tester.find_matching_constraint({'A', 'B'}, 1));
        REQUIRE(tester.find_matching_constraint({'C', 'D'}, 1));

        REQUIRE_FALSE(tester.find_matching_constraint({'A', 'B', 'C', 'D'}, 2));
    }

    TEST_CASE("Algorithm solves a variable after simplifying constraints" , "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);
        csp.add_constraint({'A', 'B', 'C'}, 2);
        auto result = csp.get_solution();
        REQUIRE(result.second == -1);
        REQUIRE(tester.update_stack_empty());
        csp.add_constraint({'A', 'B'}, 1);
        result = csp.get_solution();
        REQUIRE(result.second == 1);
        REQUIRE(result.first == 'C');
    }

    TEST_CASE("Algorithm solves constraint after giving new information" , "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);
        csp.add_constraint({'A', 'B'}, 1);
        auto result = csp.get_solution();
        REQUIRE(result.second == -1);
        csp.add_opened('A');
        result = csp.get_solution();
        REQUIRE(result.second == 1);
        REQUIRE(result.first == 'B');

        REQUIRE(tester.get_number_of_active_constraints() == 0);
    }

    TEST_CASE("Algorithm cascades solutions through constraints", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);
        csp.add_constraint({'A', 'B', 'E'}, 1);
        csp.add_constraint({'A', 'B', 'C'}, 1);
        csp.add_constraint({'A', 'C', 'D'}, 2);
        auto result = csp.get_solution();
        REQUIRE(result.second == -1);
        REQUIRE(tester.get_number_of_active_constraints() == 3);

        csp.add_opened('E');
        result = csp.get_solution();
        REQUIRE(tester.get_number_of_active_constraints() == 0);
    }
}
