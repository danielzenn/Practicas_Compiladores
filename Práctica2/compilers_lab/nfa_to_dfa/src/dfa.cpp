#include "dfa.hpp"
#include <iostream>

// Imprime el DFA: Sus estados (con elinicial y los de aceptación) y sus transiciones.
void print_dfa(const DFA& dfa) {
    std::cout << "== DFA ==\n";

    std::cout << "Estados: " << dfa.num_states << "\n";
    for (int i = 0; i < dfa.num_states; i++) {
        std::cout << "  D" << i << " = {";
        bool first = true;
        for (int s : dfa.state_labels[i]) {
            if (!first) std::cout << ", ";
            std::cout << s;
            first = false;
        }
        std::cout << "}";
        if (i == dfa.start) std::cout << "   [estado inicial]";
        if (dfa.accept.count(i)) std::cout << "   [estado de aceptacion]";
        std::cout << "\n";
    }

    std::cout << "Transiciones:\n";
    for (auto const& [key, to] : dfa.delta) {
        auto [from, sym] = key;
        std::cout << "  D" << from << " con" << sym << "va a D" << to << "\n";
    }
}

// Simula el DFA sobre un input, manteniendo un conjunto de estados activos (current)
// Por cada carácter, se calculan sus estados alcanzables 
// Regresa true si se alcanzó un estado de aceptación.
bool simulate_dfa(const DFA& dfa, const std::string& input) {
    int current = dfa.start;

    for (char c : input) {
        auto it = dfa.delta.find({current, c});

        if (it == dfa.delta.end()) {
            return false;  
        }

        current = it->second;
    }
    
    return dfa.accept.count(current) > 0;
}
