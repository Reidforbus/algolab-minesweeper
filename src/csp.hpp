#pragma once

#include <deque>
#include <memory>
#include <set>
#include <tuple>
#include <map>
#include <vector>
#include <iostream>

namespace algolab {
    template <typename var_type> class CSPGraph{
        struct Variable;

        struct Constraint {
            bool obsolete = false;
            std::set<Variable*> variables;
            int sum = -1;
            int ones_solved = 0;
            int zeroes_solved = 0;
            std::set<Variable*> unknown;
        };

        struct Variable {
            var_type name;
            std::set<Constraint*> constraints;
            int value = -1;
        };


        private:

        std::map<var_type, Variable> variables;
        std::vector<std::unique_ptr<Constraint>> constraints;

        std::deque<Constraint*> update_queue;

        std::vector<std::tuple<var_type, int>> solved;

        void solve_variable(Variable& var, int value){
            if (var.value == value) {
                // Somebody cooked here
                return;
            }

            var.value = value;
            solved.push_back(std::make_tuple(var.name, value));

            for (Constraint* ptr : var.constraints){
                Constraint& cnstrnt = *ptr;

                std::cout << cnstrnt.unknown.size() << std::endl;
                if (cnstrnt.unknown.count(&var) > 0){
                    cnstrnt.unknown.erase(&var);
                }

                if (value == 1){
                    cnstrnt.ones_solved += 1;
                } else {
                    cnstrnt.zeroes_solved += 1;
                }

                if (cnstrnt.unknown.size() > 0){
                    update_queue.push_back(&cnstrnt);
                } else {
                    cnstrnt.obsolete = true;
                }
            }

        }

        void process_queue(){
            while (!update_queue.empty()){
                // Grab a constraint from the queue
                Constraint& cnstrnt = *update_queue.front();
                update_queue.pop_front();

                // In case the constraint has been rendered obsolete before reaching the front of the queue
                if (cnstrnt.obsolete) continue;

                // If the constraint is trivial the variables are solved
                // then the constraints that include those variables
                // are updated and added onto the queue.
                // Then the process starts over

                // All variables zero
                if (cnstrnt.ones_solved == cnstrnt.sum){
                    auto vars = cnstrnt.unknown;
                    for (Variable* ptr : vars){
                        Variable& var = *ptr;
                        solve_variable(var, 0);
                    }
                    continue;

                } 

                // All variables one
                if (cnstrnt.variables.size() - cnstrnt.zeroes_solved == cnstrnt.sum){
                    auto vars = cnstrnt.unknown;
                    for (Variable* ptr : vars){
                        Variable& var = *ptr;
                        solve_variable(var, 1);
                    }
                    continue;

                }

                // For constraints that are not trivial, existing constraints that share variables
                // are checked if they are sub or supersets.
                // Then the smaller set is cut from the larger and the changed constraint is added to queue
                std::set<Constraint*> possible_supersets;
                std::set<Constraint*> possible_subsets;

                for (Variable* ptr : cnstrnt.unknown){
                    Variable& var = *ptr;
                    //TODO
                }


                //Etsi ylijoukot
                //Etsi alijoukot
                //Jaa löydetyt joukot osiin ja lisää jonoon
            }
        }

        public:

        void add_constraint(std::vector<var_type> vars, int sum){
            constraints.emplace_back(std::make_unique<Constraint>());
            Constraint* new_cnstrnt = constraints.back().get();

            for (var_type var_name : vars){
                if (variables.count(var_name) == 0){
                    variables.emplace(std::make_pair(var_name, Variable{var_name}));
                } else if (variables[var_name].value >= 0){
                    sum -= variables[var_name].value;
                    continue;
                }

                Variable& var = variables[var_name];
                var.constraints.insert(new_cnstrnt);
                new_cnstrnt->variables.insert(&var);
                new_cnstrnt->unknown.insert(&var);
            }

            new_cnstrnt->sum = sum;
            update_queue.push_back(new_cnstrnt);


            std::cout << "New constraint added, Sum: " << new_cnstrnt->sum << ", Variables: ";
            for (Variable* ptr : new_cnstrnt->variables){
                Variable& var = *ptr;
                std::cout << "(" << var.name.row << ", " << var.name.col << ") ";
            }
            std::cout << std::endl;

            process_queue();
        }

        std::tuple<var_type, int> get_solved(){
            if (solved.empty()){
                return std::make_tuple(var_type(), -1);
            }
            auto result = solved.back();
            solved.pop_back();
            return result;
        }
    };

} // namespace algolab
