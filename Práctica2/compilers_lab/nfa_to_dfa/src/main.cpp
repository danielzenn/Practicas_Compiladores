#include "nfa.hpp"
#include "dfa.hpp"
#include "subset_construction.hpp"
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

    std::cout << "----------------- NFA original -----------------\n";
    print_nfa(nfa);

    DFA dfa = subset_construction(nfa);

    std::cout << "\n----------------- DFA equivalente -----------------\n";
    print_dfa(dfa);

    std::cout << "\n----------------- Verificacion -----------------\n";
    std::vector<std::string> tests = {"", "a", "b", "aaa", "bbb", "ab", "ba", "aab", "abab"};
    bool all_ok = true;
    for (auto const& s : tests) {
        bool r_nfa = simulate_nfa(nfa, s);
        bool r_dfa = simulate_dfa(dfa, s);
        bool ok = (r_nfa == r_dfa);
        all_ok = all_ok && ok;
        std::cout << "  \"" << s << "\"  ->  NFA=" << r_nfa << "  DFA=" << r_dfa
                   << (ok ? "   OK" : "   *** Diferencia ***") << "\n";
    }

    std::cout << (all_ok ? "\nEl NFA y el DFA son equivalentes.\n" : "\nHay diferencias entre el NFA y el DFA.\n");

    return all_ok ? 0 : 1;
}
