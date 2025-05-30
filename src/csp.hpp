#include "utilities.hpp"
#include <vector>


namespace algolab {
    class CSPTrie{
        struct Node {
            Coord variable;
            std::vector<Node*> children;
            int sum = -1;

            bool operator<(const Node& a) const{
                return variable < a.variable;
            }
        };

        private:

        std::vector<Node*> root;

        bool is_trivial(std::vector<Coord>& variables, int sum){
            if (sum == 0){
                return true;
            }
            return variables.size() == sum;
        }

        Node* find_var(Coord var){
            if (root.empty()){
                return nullptr;
            }
            Node* result = nullptr;

            for (Node* node : root){
                if (node->variable>var){
                    break;
                }
                result = search(node, var);
                if (result != nullptr){
                    break;
                }
            }

            return result;
        }

        Node* search(Node* node, Coord var){
            if (node->variable == var){
                return node;
            }
            Node* result = nullptr;
            for (Node* child : node->children){
                result = search(child, var);
            }

            return result;
        }

        public:

        void add_constraint(std::vector<Coord> variables, int sum){
            if (is_trivial(variables, sum)){

            } else {

            }
        }
    };

} // namespace algolab
