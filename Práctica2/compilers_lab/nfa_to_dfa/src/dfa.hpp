#pragma once
#include <vector>
#include <set>
#include <map>
#include <string>
#include "nfa.hpp"

/*
 * Representa un DFA luego de aplicar la construcción de subconjuntos sobre un NFA.
 *
 * Mapea (estado, símbolo) a un solo estado (std::map<..., int>).
 * 'state_labels[i]' guarda el subconjunto de estados del NFA que originaron al estado i del DFA.
 */
struct DFA {
    int num_states = 0;
    std::set<char> alphabet;
    std::map<std::pair<int, char>, int> delta;
    int start = 0;
    std::set<int> accept;
    std::vector<std::set<int>> state_labels;
};

// Imprime el DFA.
void print_dfa(const DFA& dfa);

// Simula el DFA sobre un input.
bool simulate_dfa(const DFA& dfa, const std::string& input);
