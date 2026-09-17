#pragma once
#include <set>
#include <map>
#include <string>
#include <algorithm>

// Epsilon 
constexpr char EPSILON = '\0';

/*
 * Representa un NFA.
 *
 * Los estados se identifican por enteros, desde 0 hasta num_states-1.
 * 'delta' es la función de transición: (estado, símbolo) -> std::set<int> 
 * ya que puede regresar varios estados destino.
 */
struct NFA {
    int num_states = 0;
    std::set<char> alphabet;   
    std::map<std::pair<int, char>, std::set<int>> delta;
    int start = 0;
    std::set<int> accept;

    // Añade una transición al NFA, actualizando el número de estados.
    void add_transition(int from, char symbol, int to) {
        if (symbol != EPSILON) alphabet.insert(symbol);
        delta[{from, symbol}].insert(to);
        num_states = std::max({num_states, from + 1, to + 1});
    }
};

// Imprime el NFA.
void print_nfa(const NFA& nfa);

// Simula el NFA directamente sobre un input.
bool simulate_nfa(const NFA& nfa, const std::string& input);
