// Prueba del algoritmo subset-construction 
// Demuestra que subset_construction transforma correctamente un NFA con ciclos
// no-deterministas y transiciones epsilon en un DFA equivalente.
#include "../src/nfa.hpp"
#include "../src/dfa.hpp"
#include "../src/subset_construction.hpp"
#include <iostream>
#include <vector>
#include <string>

int main() {

    // NFA de ejemplo: {0, 1, 2, 3}
    // Transiciones: 0 --e--> 1     0 --e--> 2      1 --a--> 1      1 --a--> 3      2 --b--> 2      2 --b--> 3
    // Estado inicial = 0       
    // Estado de aceptación = {3}
    NFA nfa;
    nfa.start = 0;
    nfa.accept = {3};
    nfa.add_transition(0, EPSILON, 1);
    nfa.add_transition(0, EPSILON, 2);
    nfa.add_transition(1, 'a', 1);
    nfa.add_transition(1, 'a', 3);
    nfa.add_transition(2, 'b', 2);
    nfa.add_transition(2, 'b', 3);

    DFA dfa = subset_construction(nfa);
    bool ok = true;

    // Se esperan 3 estados en el DFA.
    if (dfa.num_states != 3) {
        std::cout << "FAIL: se esperaban 3 estados en el DFA resultante, hay "
                   << dfa.num_states << "\n";
        ok = false;
    } else {
        std::cout << "PASS: el DFA resultante tiene 3 estados.\n";
    }

    // Ninguna transición epsilon en el DFA.
    bool sin_epsilon = true;
    for (auto const& [key, to] : dfa.delta) {
        (void)to;
        if (key.second == EPSILON) sin_epsilon = false;
    }
    if (sin_epsilon) {
        std::cout << "PASS: el DFA no contiene transiciones epsilon.\n";
    } else {
        std::cout << "FAIL: el DFA contiene transiciones epsilon.\n";
    }
    ok = ok && sin_epsilon;

    // Equivalencia del NFA con el DFA sobre cadenas de entrada.
    std::vector<std::pair<std::string, bool>> casos = {
        {"", false}, {"a", true}, {"b", true}, {"aaa", true}, {"bbb", true},
        {"ab", false}, {"ba", false}, {"aab", false}, {"abab", false},
    };

    for (auto const& [s, esperado] : casos) {
        bool r_nfa = simulate_nfa(nfa, s);
        bool r_dfa = simulate_dfa(dfa, s);
        bool caso_ok = (r_nfa == esperado) && (r_dfa == esperado);
        std::cout << (caso_ok ? "PASS" : "FAIL")
                   << ": \"" << s << "\" esperado=" << esperado
                   << " nfa=" << r_nfa << " dfa=" << r_dfa << "\n";
        ok = ok && caso_ok;
    }

    std::cout << (ok ? "\nSe pasaron todas las pruebas :D.\n" : "\nNo se pasaron todas las pruebas :(.\n");
    return ok ? 0 : 1;
}
