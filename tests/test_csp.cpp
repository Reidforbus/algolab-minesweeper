#include <catch2/catch_test_macros.hpp>
#include <csignal>
#include <set>
#include "minesweeper/csp.hpp"


namespace algolab {
    class CSPGraphTest{
        CSPGraph<char>& csp;

        public:

        CSPGraphTest(CSPGraph<char>& c): csp(c){}

        bool find_matching_constraint(std::vector<char> vars, int sum){
            for (auto&& c : csp.constraints){
                if (c->sum != sum) continue;
                if (c->unknown.size() != vars.size()) continue;
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

        bool find_subsets(std::vector<char> vars){
            CSPGraph<char>::Constraint* cnstrnt = nullptr;
            for (auto&& c : csp.constraints){
                if (c->unknown.size() != vars.size()) continue;
                bool hit = true;
                for (char var : vars){
                    auto& var_object = csp.variables[var];
                    if (c->unknown.count(&var_object) == 0){
                        hit = false;
                        break;
                    }
                }
                if (hit){
                    cnstrnt = &*c;
                    break;
                }
            }

            return csp.find_subset(*cnstrnt) != nullptr;
        }

        int find_supersets(std::vector<char> vars){
            CSPGraph<char>::Constraint* cnstrnt = nullptr;
            for (auto&& c : csp.constraints){
                if (c->unknown.size() != vars.size()) continue;
                bool hit = true;
                for (char var : vars){
                    auto& var_object = csp.variables[var];
                    if (c->unknown.count(&var_object) == 0){
                        hit = false;
                        break;
                    }
                }
                if (hit){
                    cnstrnt = &*c;
                    break;
                }
            }
            std::set<CSPGraph<char>::Constraint*> supers;

            csp.find_supersets(*cnstrnt, supers);
            return supers.size();
        }

        std::pair<int, int> find_component_constraints(char var){
            CSPGraph<char>::Constraint* cnstrnt = nullptr;
            for (auto&& c : csp.constraints){
                auto& var_object = csp.variables[var];
                if (c->unknown.count(&var_object) > 0){
                    cnstrnt = &*c;
                    break;
                }
            }

            std::map<CSPGraph<char>::Constraint*, int> map;
            std::vector<CSPGraph<char>::Constraint*> constraints;
            std::vector<CSPGraph<char>::Variable*> variables;
            csp.component_scan(*cnstrnt, map, 1, constraints, variables);

            return std::make_pair(constraints.size(), variables.size());

        }

        bool variable_solved(char var, int val){
            for (auto [var_name, v] : csp.variables){
                if (var_name == var){
                    return v.value == val;
                }
            }
            return false;
        }

        void search_recursive(){
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

        int get_number_of_variables(){
            return csp.variables.size();
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


    TEST_CASE("Subset is found when one exists", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_constraint({'A', 'B'}, 0);
        csp.add_constraint({'A', 'B', 'C'}, 1);

        REQUIRE(tester.find_subsets({'A', 'B', 'C'}));
        REQUIRE_FALSE(tester.find_subsets({'A', 'B'}));
    }

    TEST_CASE("No subset is found when there isn't one", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_constraint({'A', 'B', 'C'}, 0);
        csp.add_constraint({'A', 'B', 'D'}, 0);
        csp.add_constraint({'C', 'B', 'D'}, 0);
        csp.add_constraint({'C', 'B', 'D'}, 0);
        csp.add_constraint({'A', 'B', 'C', 'D'}, 0);
        csp.add_constraint({'A', 'D'}, 0);
        csp.add_constraint({'B', 'D'}, 0);
        csp.add_constraint({},1);

        REQUIRE_FALSE(tester.find_subsets({'A', 'B', 'C'}));
    }

    TEST_CASE("Superset are found when they exist", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_constraint({'A', 'B', 'C'}, 0);
        csp.add_constraint({'A', 'B', 'D'}, 0);
        csp.add_constraint({'C', 'B', 'D'}, 0);
        csp.add_constraint({'C', 'B', 'D'}, 0);
        csp.add_constraint({'A', 'B', 'C', 'D'}, 0);
        csp.add_constraint({'A', 'D'}, 0);
        csp.add_constraint({'B', 'D'}, 0);
        csp.add_constraint({},1);

        REQUIRE(tester.find_supersets({'A', 'D'}) == 2);
    }

    TEST_CASE("No superset is found when there isn't one", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_constraint({'A', 'B', 'C'}, 0);
        csp.add_constraint({'A', 'B', 'D'}, 0);
        csp.add_constraint({'C', 'B', 'D'}, 0);
        csp.add_constraint({'A', 'D'}, 0);
        csp.add_constraint({'B', 'D'}, 0);
        csp.add_constraint({'D', 'C'}, 0);
        csp.add_constraint({'A', 'B'}, 0);
        csp.add_constraint({},1);

        REQUIRE(tester.find_supersets({'A', 'B' ,'C'}) == 0);
    }

    TEST_CASE("No duplicate variables are added", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_constraint({'A', 'B', 'C'}, 0);
        csp.add_constraint({'D', 'B', 'C'}, 0);
        REQUIRE(tester.get_number_of_variables() == 4);
        csp.add_constraint({'E', 'F'}, 1);
        REQUIRE(tester.get_number_of_variables() == 6);
        csp.add_opened('E');

        REQUIRE(tester.get_number_of_variables() == 6);

    }

    TEST_CASE("New variable is added when opened", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_constraint({'A', 'B', 'C'}, 0);
        REQUIRE(tester.get_number_of_variables() == 3);
        csp.add_opened('D');
        REQUIRE(tester.get_number_of_variables() == 4);
    }

    TEST_CASE("Variable is automatically removed from constraint if already solved", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_opened('D');
        csp.add_constraint({'A', 'B', 'D'}, 1);
        REQUIRE(tester.find_matching_constraint({'A', 'B'}, 1));
        REQUIRE_FALSE(tester.find_matching_constraint({'A', 'B', 'C'}, 1));
    }

    TEST_CASE("Component scan finds all connected constraints", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.add_constraint({'A', 'B', 'C'}, 0);
        csp.add_constraint({'A', 'B', 'D'}, 0);
        csp.add_constraint({'C', 'B', 'D'}, 0);
        csp.add_constraint({'A', 'D'}, 0);
        csp.add_constraint({'B', 'D'}, 0);
        csp.add_constraint({'D', 'C'}, 0);
        csp.add_constraint({'A', 'B'}, 0);
        csp.add_constraint({'E', 'F'}, 0);
        csp.add_constraint({'E', 'G'}, 0);
        csp.add_constraint({'G', 'F'}, 0);
        csp.add_constraint({'H', 'F'}, 0);
        csp.add_constraint({'Q'}, 0);

        REQUIRE(tester.find_component_constraints('A') == std::make_pair(7, 4));
        REQUIRE(tester.find_component_constraints('E') == std::make_pair(4, 4));
        REQUIRE(tester.find_component_constraints('Q') == std::make_pair(1, 1));

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

    TEST_CASE("Recursive search finds trivial solution", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.update_remaining_ones(99);
        csp.add_constraint({'A','B','C'}, 0);
        csp.add_constraint({'C', 'D'}, 0);

        REQUIRE(csp.brute_force_search());
        REQUIRE(tester.get_number_of_active_constraints() == 0);

        CSPGraph<char> csp2;
        CSPGraphTest tester2(csp2);

        csp2.update_remaining_ones(99);
        csp2.add_constraint({'A','B','C'}, 3);
        csp2.add_constraint({'C', 'D'}, 2);

        REQUIRE(csp2.brute_force_search());
        REQUIRE(tester2.get_number_of_active_constraints() == 0);
    }

    TEST_CASE("Recursive search solves overlap", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.update_remaining_ones(99);
        csp.add_constraint({'A', 'B', 'C'}, 2);
        csp.add_constraint({'B', 'C', 'D'}, 1);

        REQUIRE(csp.brute_force_search());
        REQUIRE(tester.variable_solved('D', 0));
        REQUIRE(tester.variable_solved('A', 1));
    }

    TEST_CASE("Recursive search solves chain", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.update_remaining_ones(99);
        csp.add_constraint({'A', 'B', 'C'}, 2);
        csp.add_constraint({'B', 'C', 'D'}, 1);
        csp.add_constraint({'D', 'E'}, 1);
        csp.add_constraint({'F', 'E'}, 1);
        csp.add_constraint({'F', 'G'}, 1);

        REQUIRE(csp.brute_force_search());
        REQUIRE(tester.get_number_of_active_constraints() == 2);
    }
    TEST_CASE("Recursive doesn't solve 50-50", "[CSP]"){
        CSPGraph<char> csp;
        CSPGraphTest tester(csp);

        csp.update_remaining_ones(99);
        csp.add_constraint({'A', 'B'}, 1);
        csp.add_constraint({'C', 'D'}, 1);
        csp.add_constraint({'A', 'C'}, 1);
        csp.add_constraint({'B', 'D'}, 1);

        REQUIRE_FALSE(csp.brute_force_search());
        REQUIRE(tester.get_number_of_active_constraints() == 4);
    }
}
