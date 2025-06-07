#pragma once

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
            std::set<Variable*> initial_vars;
            int sum = -1;
            std::set<Variable*> unknown;

            void print() const{
                std::cout << "Sum: " << sum << " ";
                for (Variable* v_ptr : initial_vars){
                    if (unknown.count(v_ptr) == 1){
                        std::cout << "(" << v_ptr->name.row << " " << v_ptr->name.col << ") ";
                    }
                }
                std::cout << std::endl;
            }
        };

        struct Variable {
            var_type name;
            std::set<Constraint*> constraints;
            int value = -1;
        };


        private:

        std::map<var_type, Variable> variables;
        std::vector<std::unique_ptr<Constraint>> constraints;

        std::vector<Constraint*> update_stack;

        std::vector<std::tuple<var_type, int>> solved;

        void solve_variable(Variable& var, int value){
            if (var.value != -1) {
                // Somebody cooked here
                return;
            }

            var.value = value;
            solved.push_back(std::make_tuple(var.name, value));

            auto cs = var.constraints;
            for (Constraint* ptr : cs){
                Constraint& cnstrnt = *ptr;

                cnstrnt.unknown.erase(&var);
                var.constraints.erase(&cnstrnt);
                cnstrnt.sum -= value;

                if (cnstrnt.unknown.size() > 0){
                    update_stack.push_back(&cnstrnt);
                } else {
                    cnstrnt.obsolete = true;
                }
            }

        }

        void find_solutions(){
            while (!update_stack.empty()){
                // Grab a constraint from the stack
                Constraint& cnstrnt = *update_stack.back();
                update_stack.pop_back();

                // In case the constraint has been rendered obsolete before reaching top of stack
                if (cnstrnt.unknown.size() == 0) continue;

                // If the constraint is trivial the variables are solved
                // then the constraints that include those variables
                // are updated and added onto the stack
                // Then the process starts over

                // All variables zero
                if (cnstrnt.sum == 0){
                    auto vars = cnstrnt.unknown;
                    for (Variable* ptr : vars){
                        Variable& var = *ptr;
                        solve_variable(var, 0);
                    }
                    continue;

                } 

                // All variables one
                if (cnstrnt.unknown.size() == cnstrnt.sum){
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

                // Subsets
                Constraint* subset = nullptr;

                for (Variable* v_ptr : cnstrnt.unknown){
                    for (Constraint* c_ptr : v_ptr->constraints){
                        Constraint& c = *c_ptr;
                        if ((c.unknown.size() >= cnstrnt.unknown.size()) | (c.unknown.size() == 0)) continue;

                        bool valid = true;
                        for (Variable* v2_ptr : c.unknown){
                            if (cnstrnt.unknown.count(v2_ptr) == 0){
                                valid = false;
                                break;
                            }
                        }
                        if (valid){
                            subset = &c;
                            break;
                        }
                    }
                    if (subset != nullptr){
                        break;
                    }
                }

                if (subset != nullptr){

                    cnstrnt.sum -= subset->sum;

                    for (Variable* v_ptr : subset->unknown){
                        cnstrnt.unknown.erase(v_ptr);
                        v_ptr->constraints.erase(&cnstrnt);
                    }
                    update_stack.push_back(&cnstrnt);
                    continue;
                }

                // Supersets
                std::set<Constraint*> supersets;

                // Initially all constraints that share the first variable
                // and are larger are added to the set
                auto vars = cnstrnt.unknown.cbegin();
                Variable& first = **vars++;
                for (Constraint* c_ptr : first.constraints){
                    if (c_ptr->unknown.size() > cnstrnt.unknown.size()){
                        if (c_ptr->unknown.count(&first) > 0) supersets.insert(c_ptr);
                    }

                }

                // Then the constraint are checked for each subsequent variable
                for (;vars != cnstrnt.unknown.cend();vars++){
                    Variable* v_ptr = *vars;
                    for (auto iter = supersets.begin(); iter != supersets.end();){
                        Constraint* c_ptr = *iter;
                        if (c_ptr->unknown.count(v_ptr) > 0){
                            iter++;
                        } else {
                            iter = supersets.erase(iter);
                        }
                    }
                }

                // If any supersets are found, their sum is reduced and variables cut

                if (supersets.size() > 0){
                    for (Constraint* c_ptr : supersets){
                        Constraint& c = *c_ptr;

                        c.sum  -= cnstrnt.sum;

                        auto vars = cnstrnt.unknown;
                        for (Variable* v_ptr : vars){
                            c.unknown.erase(v_ptr);
                            v_ptr->constraints.erase(&c);
                        }

                        update_stack.push_back(&c);
                    }
                    continue;
                }
            }

            //TODO: When queue is empty and no solutions are ready,
            // move to a backtracking search for all possible solutions
            // and find common ones and zeroes
        }

        public:

        void add_opened(var_type var_name){
            if (variables.count(var_name) == 0){
                variables.emplace(std::make_pair(var_name, Variable{var_name}));
            }
            Variable& var = variables[var_name];
            solve_variable(var, 0);
            find_solutions();
        }

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
                new_cnstrnt->initial_vars.insert(&var);
                new_cnstrnt->unknown.insert(&var);
            }

            new_cnstrnt->sum = sum;
            update_stack.push_back(new_cnstrnt);
        }

        std::tuple<var_type, int> get_solution(){
            if (solved.empty()){
                return std::make_tuple(var_type(), -1);
            }
            auto result = solved.back();
            solved.pop_back();
            return result;
        }
    };

} // namespace algolab
