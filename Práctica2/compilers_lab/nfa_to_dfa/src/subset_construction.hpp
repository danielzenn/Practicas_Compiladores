#pragma once
#include <set>
#include "nfa.hpp"
#include "dfa.hpp"

// Algoritmo 2: Cerradura-epsilon de un conjunto de estados T.
std::set<int> epsilon_closure(const NFA& nfa, const std::set<int>& T);

// Algoritmo 1: Conjunto de estados alcanzables desde T con el simbolo a.
std::set<int> move_set(const NFA& nfa, const std::set<int>& T, char a);

// Algoritmo 3: A partir de un NFA construye el DFA equivalente.
DFA subset_construction(const NFA& nfa);
