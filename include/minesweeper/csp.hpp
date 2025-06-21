#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <map>
#include <vector>
#include <iostream>

namespace algolab {

    const uint64_t ONE = 1;

    template <typename var_type> class CSPGraph{
        friend class CSPGraphTest;
        struct Variable;

        struct Constraint {
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

            bool obsolete(){
                return unknown.size() == 0;
            }
        };

        struct Variable {
            var_type name;
            std::set<Constraint*> constraints;
            int value = -1;
        };

        struct Constraint_Mask {
            uint64_t mask;
            int sum;
        };

        private:

        std::map<var_type, Variable> variables;
        std::vector<std::unique_ptr<Constraint>> constraints;

        std::vector<Constraint*> update_stack;

        std::vector<std::pair<var_type, int>> solved;


        int max_n = -1;

        void solve_variable(Variable& var, int value){
            if (var.value != -1) {
                // Somebody cooked here
                return;
            }

            var.value = value;
            solved.push_back(std::make_pair(var.name, value));

            auto cs = var.constraints;
            for (Constraint* ptr : cs){
                Constraint& cnstrnt = *ptr;

                cnstrnt.unknown.erase(&var);
                var.constraints.erase(&cnstrnt);
                cnstrnt.sum -= value;

                if (!cnstrnt.obsolete()){
                    update_stack.push_back(&cnstrnt);
                }
            }

        }

        void component_scan(Constraint& c, std::map<Constraint*, int>& components, int id, std::vector<Constraint*>& c_vec, std::vector<Variable*>& v_vec){
                if (components.count(&c)){
                    return;
                }
                components[&c] = id;
                c_vec.push_back(&c);

                for (Variable* v_ptr : c.unknown){
                    bool found = false;
                    for (Variable* v2_ptr : v_vec){
                        if (v2_ptr == v_ptr){
                            found = true;
                        }
                    }
                    if (!found){
                        v_vec.push_back(v_ptr);
                    }
                    for (Constraint* next_c_ptr : v_ptr->constraints){
                        component_scan(*next_c_ptr, components, id, c_vec, v_vec);
                    }
                }
        }

        void search_recursively(uint64_t partial_solution, int depth, int n, std::vector<uint64_t>& solution_vec, std::vector<Constraint_Mask>& masks){
            if (depth == n){
                for (Constraint_Mask mask : masks){
                    if (std::__popcount(partial_solution & mask.mask) != mask.sum) return;
                }
                solution_vec.push_back(partial_solution);
                return;
            }

            for (Constraint_Mask mask : masks){
                if (std::__popcount(partial_solution & mask.mask) > mask.sum) return;
            }


            search_recursively(partial_solution + (ONE << depth), depth + 1, n, solution_vec, masks);
            search_recursively(partial_solution, depth + 1, n, solution_vec, masks);
        }

        bool divide_and_search(){
            bool found_solutions = false;

            int component_id = 0;
            std::map<Constraint*, int> component_map;

            std::vector<std::vector<Constraint*>> comp_constraints;
            std::vector<std::vector<Variable*>> comp_variables;

            for (auto&& uniq_c_ptr : constraints){
                Constraint& cnstrnt = *uniq_c_ptr;
                if (cnstrnt.obsolete()) continue;

                if (!component_map.count(&cnstrnt)){
                    component_scan(cnstrnt, component_map, ++component_id, comp_constraints.emplace_back(), comp_variables.emplace_back());
                }
            }


            for (int id = 0; id < component_id; id++){
                if (comp_constraints[id].size() < 2) continue;
                int n = comp_variables[id].size();

                if (n > 64) {
                    continue;
                }

                std::vector<Constraint_Mask> masks;
                for (Constraint* c_ptr : comp_constraints[id]){
                    uint64_t mask = 0;
                    for (int i = 0; i < n; i++){
                        if (c_ptr->unknown.count(comp_variables[id][i])){
                            mask += (ONE << i);
                        }
                    }
                    masks.emplace_back(Constraint_Mask{mask, c_ptr->sum});
                }

                std::vector<uint64_t> solutions;
                search_recursively(0, 0, n, solutions, masks);

                if (solutions.empty()) continue;

                uint64_t ones = (ONE << n) - 1;
                uint64_t zeroes = (ONE << n) - 1;
                bool valid = false;

                for (uint64_t solution : solutions){
                    if (std::__popcount(solution) > max_n){
                        continue;
                    }
                    valid = true;
                    ones &= solution;
                    zeroes &= ~solution;
                }
                if (!valid) continue;

                if (ones != 0){
                    for (int i = 0; i < n; i++){
                        if (ones & (ONE << i)){
                            Variable& var = *comp_variables[id][i];
                            found_solutions = true;
                            solve_variable(var, 1);
                        }
                    }
                }

                if (ones != 0){
                    for (int i = 0; i < n; i++){
                        if (zeroes & (ONE << i)){
                            Variable& var = *comp_variables[id][i];
                            found_solutions = true;
                            solve_variable(var, 0);
                        }
                    }
                }



            }

            return found_solutions;
        }

        inline bool trivial_all_zeroes(Constraint& c){
                if (c.sum == 0){
                    auto vars = c.unknown;
                    for (Variable* ptr : vars){
                        Variable& var = *ptr;
                        solve_variable(var, 0);
                    }
                    return true;
                } 
                return false;
        }

        inline bool trivial_all_ones(Constraint& c){
                if (c.unknown.size() == c.sum){
                    auto vars = c.unknown;
                    for (Variable* ptr : vars){
                        Variable& var = *ptr;
                        solve_variable(var, 1);
                    }
                    return true;
                } 
                return false;
        }

        inline Constraint* find_subset(Constraint& cnstrnt){
                for (Variable* v_ptr : cnstrnt.unknown){
                    for (Constraint* c_ptr : v_ptr->constraints){
                        Constraint& c = *c_ptr;
                        if ((c.unknown.size() >= cnstrnt.unknown.size()) || c.obsolete()) continue;

                        bool valid = true;
                        for (Variable* v2_ptr : c.unknown){
                            if (cnstrnt.unknown.count(v2_ptr) == 0){
                                valid = false;
                                break;
                            }
                        }
                        if (valid){
                            return &c;
                        }
                    }
                }
                return nullptr;
        }

        inline void reduce_with_subset(Constraint& cnstrnt, Constraint& subset){
            cnstrnt.sum -= subset.sum;

            for (Variable* v_ptr : subset.unknown){
                cnstrnt.unknown.erase(v_ptr);
                v_ptr->constraints.erase(&cnstrnt);
            }
            if (!cnstrnt.obsolete()){
                update_stack.push_back(&cnstrnt);
            }
        }

        inline void find_supersets(Constraint& cnstrnt, std::set<Constraint*>& supersets){
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
        }

        inline void reduce_supersets(Constraint& cnstrnt, std::set<Constraint*>& supersets){
            for (Constraint* c_ptr : supersets){
                Constraint& c = *c_ptr;

                c.sum  -= cnstrnt.sum;

                auto vars = cnstrnt.unknown;
                for (Variable* v_ptr : vars){
                    c.unknown.erase(v_ptr);
                    v_ptr->constraints.erase(&c);
                }

                if (!c.obsolete()){
                    update_stack.push_back(&c);
                }
            }
        }

        void find_solutions(){
            while (!update_stack.empty()){
                // Grab a constraint from the stack
                Constraint& cnstrnt = *update_stack.back();
                update_stack.pop_back();

                // In case the constraint has been rendered obsolete before reaching top of stack
                if (cnstrnt.obsolete()) continue;

                // If the constraint is trivial the variables are solved
                // then the constraints that include those variables
                // are updated and added onto the stack
                // Then the process starts over

                // All variables zero
                if (trivial_all_zeroes(cnstrnt)) continue;

                // All variables one
                if (trivial_all_ones(cnstrnt)) continue;

                // For constraints that are not trivial, existing constraints that share variables
                // are checked if they are sub or supersets.
                // Then the smaller set is cut from the larger and the changed constraint is added to queue

                // Subsets
                Constraint* subset = find_subset(cnstrnt);

                if (subset != nullptr){
                    reduce_with_subset(cnstrnt, *subset);
                    continue;
                }

                // Supersets
                std::set<Constraint*> supersets;
                find_supersets(cnstrnt, supersets);

                // If any supersets are found, their sum is reduced and variables cut

                if (supersets.size() > 0){
                    reduce_supersets(cnstrnt, supersets);
                    continue;
                }
            }


        }

        public:

        void update_remaining_ones(int n){
            max_n = n; 
        }

        bool brute_force_search(){
            return divide_and_search();
        }

        void print_unsolved() const{
            for (auto& c : constraints){
                if (!c->obsolete()) c->print();
            }
        }

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

        std::pair<var_type, int> get_solution(){
            find_solutions();
            if (solved.empty()){
                return std::make_pair(var_type(), -1);
            }
            auto result = solved.back();
            solved.pop_back();
            return result;
        }
    };

} // namespace algolab
