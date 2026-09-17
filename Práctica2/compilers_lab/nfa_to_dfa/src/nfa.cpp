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
        std::cout << "  " << from << " con "
                   << (sym == EPSILON ? std::string("e") : std::string(1, sym))
                   << " va a {";
        bool first = true;
        for (int d : destinations) {
            if (!first) std::cout << ", ";
            std::cout << d;
            first = false;
        }
        std::cout << "}\n";
    }
}

// Cerradura epsilon interno (para simular el NFA ).
static std::set<int> epsilon_closure_nfa(const NFA& nfa, const std::set<int>& T) {
    std::set<int> C(T.begin(), T.end());
    std::stack<int> pila;
    for (int t : T) pila.push(t);

    while (!pila.empty()) {
        int t = pila.top();
        pila.pop();
        auto it = nfa.delta.find({t, EPSILON});

        if (it != nfa.delta.end()) {

            for (int u : it->second) {
                if (!C.count(u)) {
                    C.insert(u);
                    pila.push(u);
                }
            }
        }
    }

    return C;
}

// Simula el NFA sobre un input, manteniendo un conjunto de estados activos (current)
// Por cada carácter, se calculan sus estados alcanzables y e agrega además su cerradura epsilon. 
// Regresa true si algún estado posible es de aceptación.
bool simulate_nfa(const NFA& nfa, const std::string& input) {
    std::set<int> current = epsilon_closure_nfa(nfa, {nfa.start});

    for (char c : input) {
        std::set<int> next;

        for (int s : current) {
            auto it = nfa.delta.find({s, c});

            if (it != nfa.delta.end()) {
                next.insert(it->second.begin(), it->second.end());
            }   
        }

        current = epsilon_closure_nfa(nfa, next);
    }

    for (int s : current) {
        if (nfa.accept.count(s)) return true;
    }

    return false;
}
