#pragma once

#include <deque>
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
        };

        struct Variable {
            var_type name;
            std::set<Constraint*> constraints;
            int value = -1;

            bool operator<(const Variable& a) const{
                return name < a.name;
            }
        };


        private:

        std::map<var_type, Variable> variables;
        std::vector<Constraint> constraints;

        std::deque<Constraint*> update_queue;

        std::vector<std::tuple<var_type, int>> solved;

        void solve_variable(Variable& var, int value){
            var.value = value;
            solved.push_back(std::make_tuple(var.name, value));

            for (Constraint* ptr : var.constraints){
                Constraint& cnstrnt = *ptr;

                cnstrnt.variables.erase(&var);
                // TODO: generates segfaults probably when variable is already erased from constraint
                cnstrnt.sum -= value;
                if (cnstrnt.variables.size() > 0){
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
                if (cnstrnt.sum == 0){
                    auto vars = cnstrnt.variables;
                    for (Variable* ptr : vars){
                        Variable& var = *ptr;
                        solve_variable(var, 0);
                    }
                    continue;

                } 

                // All variables one
                if (cnstrnt.variables.size() == cnstrnt.sum){
                    auto vars = cnstrnt.variables;
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

                for (Variable* ptr : cnstrnt.variables){
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
            constraints.push_back(Constraint());
            Constraint& new_cnstrnt = constraints.back();

            for (var_type var_name : vars){
                if (variables.count(var_name) == 0){
                    variables[var_name] = Variable{var_name};
                } else if (variables[var_name].value >= 0){
                    sum -= variables[var_name].value;
                    continue;
                }

                Variable& var = variables[var_name];
                var.constraints.insert(&new_cnstrnt);
                new_cnstrnt.variables.insert(&var);
            }
            new_cnstrnt.sum = sum;
            update_queue.push_back(&new_cnstrnt);

            std::cout << "New constraint added, Sum: " << new_cnstrnt.sum << ", Variables: ";
            for (Variable* ptr : new_cnstrnt.variables){
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
