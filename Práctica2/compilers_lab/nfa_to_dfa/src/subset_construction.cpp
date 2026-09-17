#include "subset_construction.hpp"
#include <stack>
#include <queue>
#include <map>

/*
 * Algoritmo 2: Cerradura-epsilon de un conjunto de estados T.
 *
 * Se inicializa una pila con los estados de T y un conjunto de estados inicial C = T.
 * Mientras la pila no esté vacía, se saca un estado, se buscan sus transiciones epsilon y si no están ya en C se agregan.
 */
std::set<int> epsilon_closure(const NFA& nfa, const std::set<int>& T) {
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

/*
 * Algoritmo 1: Conjunto de estados alcanzables desde T con el simbolo a.
 * 
 * Se inicializa un conjunto de estados inicial R.
 * Se recorren los estados de T y por cada uno se agregan los estados alcanzables con el símbolo a a R.
 */
std::set<int> move_set(const NFA& nfa, const std::set<int>& T, char a) {
    std::set<int> R;

    for (int s : T) {
        auto it = nfa.delta.find({s, a});

        if (it != nfa.delta.end()) {
            R.insert(it->second.begin(), it->second.end());
        }
    }

    return R;
}

/*
 * Algoritmo 3: A partir de un NFA construye el DFA equivalente.

 * Se crea un DFA vacío e el que se copia el alfabeto del NFA.
 * Se inicializa diccionario para representar los subconjuntos de estados del NFA como estados del DFA
 * y una cola de subconjuntos por procesar inicializada con el estado inicial.
 * 
 * Mientras la cola no esté vacía se desencola un elemento, se obtienen diferentes V como la cerradura epsilon del conjunto de estados 
 * alcanzables desde ese elemento (conjunto de estados) con cada letra del alfabeto. Si V no está en el diccionario de estados se mete
 * y se agrega a la cola. Se agregan las transiciones de V.
 * 
 * Por último, se agregan como estados finales del DFA si contienen a algún estado final del NFA.
 */
DFA subset_construction(const NFA& nfa) {
    DFA dfa;
    dfa.alphabet = nfa.alphabet;

    std::map<std::set<int>, int> state_id;   
    std::queue<std::set<int>> cola;

    std::set<int> s0 = epsilon_closure(nfa, {nfa.start});
    state_id[s0] = 0;
    dfa.state_labels.push_back(s0);
    cola.push(s0);

    while (!cola.empty()) {
        std::set<int> U = cola.front();
        cola.pop();
        int uid = state_id[U];

        for (char a : nfa.alphabet) {
            std::set<int> V = epsilon_closure(nfa, move_set(nfa, U, a));

            if (V.empty()) continue; 

            if (!state_id.count(V)) {
                int new_id = (int) dfa.state_labels.size();
                state_id[V] = new_id;
                dfa.state_labels.push_back(V);
                cola.push(V);
            }
            
            dfa.delta[{uid, a}] = state_id[V];
        }
    }

    dfa.num_states = (int) dfa.state_labels.size();
    dfa.start = 0;

    for (int i = 0; i < dfa.num_states; i++) {

        for (int s : dfa.state_labels[i]) {

            if (nfa.accept.count(s)) {
                dfa.accept.insert(i);
                break;
            }
        }
    }

    return dfa;
}

