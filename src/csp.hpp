#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace algolab {
    template <typename var_type> class CSPGraph{
        struct Variable;

        struct Constraint {
            std::set<Variable&> variables;
            int sum = -1;
            int ones_solved = 0;
            std::set<Variable&> unknown;
        };

        struct Variable {
            var_type name;
            std::set<Constraint&> constraints;
            int value = -1;
        };


        private:

        std::unordered_map<var_type, Variable> variables;
        std::vector<Constraint> constraints;

        std::vector<Constraint&> update_queue;

        std::vector<std::tuple<var_type, int>> solved;

        void process_queue(){
            while (!update_queue.empty()){
            }
        }

        public:

        void add_constraint(std::vector<var_type> vars, int sum){
            constraints.push_back(Constraint());
            Constraint& new_cnstrnt = constraints.back();
            new_cnstrnt.sum = sum;

            for (var_type var_name : vars){
                if (variables.count(var_name) == 0){
                    variables.insert(var_name, Variable(var_name));
                } 
                Variable& var = variables[var_name];
                var.constraints.insert(new_cnstrnt);
                new_cnstrnt.variables.insert(var);
                new_cnstrnt.unknown.insert(var);
            }
            update_queue.insert(new_cnstrnt);

            process_queue();
        }

        std::tuple<var_type, int> get_solved(){
            if (solved.empty()){
                return std::tie(var_type(), -1);
            }
            auto result = solved.back();
            solved.pop_back();
            return result;
        }
    };

} // namespace algolab
