#include "nfa.h"
#include <stdlib.h>
#include <string.h>

/*
 * Representa un fragmento de Thompson, un NFA parcial con un estado inicial y uno de aceptación. 
 * Los fragmentos se van combinando según el operador visto hasta formar el NFA completo.
 */
typedef struct {
    int start;
    int accept;
} fragment;

/*
 * Agrega un nuevo estado vacío dentro del NFA, reservando más memoria si es necesario, y regresa su índice.
 */
static int new_state(nfa* n) {
    if (n->num_states >= n->capacity) {
        n->capacity *= 2;
        n->states = realloc(n->states, sizeof(nfa_state) * n->capacity);
    }
    int id = n->num_states++;
    n->states[id].num_transitions = 0;
    n->states[id].to[0] = n->states[id].to[1] = -1;
    n->states[id].symbol[0] = n->states[id].symbol[1] = EPSILON;
    return id;
}

/*
 * Agrega una transición al estado 'from' en el siguiente espacio libre de to[] o symbol[] y avamza el contador.
 */
static void add_transition(nfa* n, int from, char symbol, int to) {
    nfa_state* s = &n->states[from];
    s->to[s->num_transitions] = to;
    s->symbol[s->num_transitions] = symbol;
    s->num_transitions++;
}

/*
 * Convierte un regex en notación postfija con concatenación explícita
 * a un NFA equivalente mediante el algoritmo de Thompson.
 * Se recorre en postfijo token por token usando una pila de fragmentos,
 * cada token saca de la pila los fragmentos que necesita, los combina
 * según su tipo y apila el fragmento resultante.
 * Finalmente, solo quedará el NFA completo y se liberará la memoria de la pila.
 */
nfa regex_to_nfa(regex r) {
    nfa n;
    n.capacity = r.size * 2 + 4;
    n.states = malloc(sizeof(nfa_state) * n.capacity);
    n.num_states = 0;
    n.start = -1;
    n.accept = -1;

    fragment* stack = malloc(sizeof(fragment) * (r.size + 1));
    int top = -1;

    for (int i = 0; i < r.size; i++) {
        regex_item current_token = r.items[i];

        if (current_token.type == TOKEN_CHAR) {
            int s0 = new_state(&n);
            int s1 = new_state(&n);
            add_transition(&n, s0, current_token.value, s1);
            stack[++top] = (fragment){s0, s1};

        } else if (current_token.type == TOKEN_CONCAT) {
            fragment f2 = stack[top--];
            fragment f1 = stack[top--];
            add_transition(&n, f1.accept, EPSILON, f2.start);
            stack[++top] = (fragment){f1.start, f2.accept};

        } else if (current_token.type == TOKEN_UNION) {
            fragment f2 = stack[top--];
            fragment f1 = stack[top--];
            int s = new_state(&n);
            int t = new_state(&n);
            add_transition(&n, s, EPSILON, f1.start);
            add_transition(&n, s, EPSILON, f2.start);
            add_transition(&n, f1.accept, EPSILON, t);
            add_transition(&n, f2.accept, EPSILON, t);
            stack[++top] = (fragment){s, t};

        } else if (current_token.type == TOKEN_STAR) {
            fragment f = stack[top--];
            int s = new_state(&n);
            int t = new_state(&n);
            add_transition(&n, s, EPSILON, f.start);
            add_transition(&n, s, EPSILON, t);
            add_transition(&n, f.accept, EPSILON, f.start);
            add_transition(&n, f.accept, EPSILON, t);
            stack[++top] = (fragment){s, t};

        } else if (current_token.type == TOKEN_PLUS) {
            fragment f = stack[top--];
            int t = new_state(&n);
            add_transition(&n, f.accept, EPSILON, f.start);
            add_transition(&n, f.accept, EPSILON, t);
            stack[++top] = (fragment){f.start, t};

        } else if (current_token.type == TOKEN_QUESTION) {
            fragment f = stack[top--];
            int s = new_state(&n);
            int t = new_state(&n);
            add_transition(&n, s, EPSILON, f.start);
            add_transition(&n, s, EPSILON, t);
            add_transition(&n, f.accept, EPSILON, t);
            stack[++top] = (fragment){s, t};
        }
    }

    if (top == 0) {
        n.start = stack[0].start;
        n.accept = stack[0].accept;
    }

    free(stack);
    return n;
}

/*
 * Libera la memoria reservada por la construcción de cualquier NFA representado. 
 */
void free_nfa(nfa* n) {
    free(n->states);
    n->states = NULL;
    n->num_states = 0;
    n->capacity = 0;
}


/*
 * Realiza la cerradura epsilon de un conjunto de estados, agregando todos los estados alcanzables 
 * mediante transiciones epsilon a current[].
 */
static void epsilon_closure(nfa* n, char* current) {
    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < n->num_states; i++) {
            if (!current[i]) continue;
            nfa_state* s = &n->states[i];
            for (int t = 0; t < s->num_transitions; t++) {
                if (s->symbol[t] == EPSILON && s->to[t] != -1 &&
                    !current[s->to[t]]) {
                    current[s->to[t]] = 1;
                    changed = 1;
                }
            }
        }
    }
}

/* Simula el NFA sobre un input y regresa 1 si la cadena es aceptada, 0 de lo contrario. 
 * Mantiene un conjunto de estados activos (current) y por cada carácter, se calcula el 
 * siguiente conjunto (next) siguiendo las transiciones que coincidan con ese carácter,
 * Se recalcula el cierre epsilon después de cada paso.
 * Finalmente, se verifica el estado de aceptación y se libera la memoria de los conjuntos.
 */
int match_nfa(nfa n, const char* str, int len) {
    if (n.num_states == 0 || n.start == -1) return 0;

    char* current = calloc(n.num_states, sizeof(char));
    char* next = calloc(n.num_states, sizeof(char));

    current[n.start] = 1;
    epsilon_closure(&n, current);

    for (int i = 0; i < len; i++) {
        memset(next, 0, n.num_states);
        char c = str[i];
        for (int s = 0; s < n.num_states; s++) {
            if (!current[s]) continue;
            nfa_state* st = &n.states[s];
            for (int t = 0; t < st->num_transitions; t++) {
                if (st->symbol[t] == c && st->to[t] != -1) {
                    next[st->to[t]] = 1;
                }
            }
        }
        epsilon_closure(&n, next);
        memcpy(current, next, n.num_states);
    }

    int accepted = current[n.accept];
    free(current);
    free(next);
    return accepted;
}
