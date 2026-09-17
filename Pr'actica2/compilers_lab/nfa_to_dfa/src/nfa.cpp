#include "nfa.hpp"
#include <iostream>
#include <stack>

// Imprime el NFA: Sus estados, su estado inicial, sus estados de aceptación y sus transiciones.
void print_nfa(const NFA& nfa) {
    std::cout << "== NFA ==\n";

    std::cout << "Estados: " << nfa.num_states << " (0.." << nfa.num_states - 1 << ")\n";

    std::cout << "Estado inicial: " << nfa.start << "\n";

    std::cout << "Estados de aceptacion: {";
    bool first = true;
    for (int s : nfa.accept) {
        if (!first) std::cout << ", ";
        std::cout << s;
        first = false;
    }
    std::cout << "}\n";

    std::cout << "Transiciones:\n";
    for (auto const& [key, destinations] : nfa.delta) {
        auto [from, sym] = key;
        std::cout << "  " << from << " con"
                   << (sym == EPSILON ? std::string("e") : std::string(1, sym))
                   << "va a {";
        bool first = true;
        for (int d : destinations) {
            if (!first) std::cout << ", ";
            std::cout << d;
            first = false;
        }
        std::cout << "}\n";
    }
}

