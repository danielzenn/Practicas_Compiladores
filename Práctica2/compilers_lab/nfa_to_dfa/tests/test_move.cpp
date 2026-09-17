// Prueba del algoritmo move-set (move)
#include "../src/nfa.hpp"
#include "../src/subset_construction.hpp"
#include <iostream>

int main() {

    // NFA con estados {0, 1, 2}, transiciones 0-a->1, 0-a->2, 0-b->3.
    NFA nfa;
    nfa.add_transition(0, 'a', 1);
    nfa.add_transition(0, 'a', 2);
    nfa.add_transition(0, 'b', 3);

    std::set<int> result = move_set(nfa, {0}, 'a');
    std::set<int> expected = {1, 2};

    // move({0}, 'a') debería regresar {1,2}.
    if (result == expected) {
        std::cout << "PASS: move({0}, 'a') = {1,2}\n";
        return 0;
    }

    std::cout << "FAIL: Resultado esperado: {1,2}. Resultado obtenido: {";
    for (int s : result) std::cout << s << " ";
    std::cout << "}\n";
    return 1;
}
