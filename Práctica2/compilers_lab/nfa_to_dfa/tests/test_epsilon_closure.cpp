// Prueba del algoritmo epsilon-closure.
#include "../src/nfa.hpp"
#include "../src/subset_construction.hpp"
#include <iostream>

int main() {

    // NFA con estados {0,1,2,3}, transiciones 0-e->1, 1-e->2, ciclo 2-e->1.
    NFA nfa;
    nfa.add_transition(0, EPSILON, 1);
    nfa.add_transition(1, EPSILON, 2);
    nfa.add_transition(2, EPSILON, 1);  
    nfa.num_states = 4;               

    std::set<int> result = epsilon_closure(nfa, {0});
    std::set<int> expected = {0, 1, 2};

    // epsilon-closure(0) debería regresar {0, 1, 2}.
    if (result == expected) {
        std::cout << "PASS: epsilon_closure({0}) = {0,1,2}\n";
        return 0;
    }

    std::cout << "FAIL: Resultado esperado: {0,1,2}. Resultado obtenido {";
    for (int s : result) std::cout << s << " ";
    std::cout << "}\n";
    return 1;
}
