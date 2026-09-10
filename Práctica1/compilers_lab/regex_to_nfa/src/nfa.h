#ifndef NFA_H
#define NFA_H

#include "regex.h"

/* Por las reglas de construcción de Thompson, cada estado creado
 * tiene a lo más 2 transiciones salientes (@).
 */

/* 
 * Carácter nulo para transiciones epsilon.
 */
#define EPSILON '\0'

/* 
 * Un estado de un NFA. Se usa (@) para garantizar a lo más 2 transiciones salientes.
 */
typedef struct {
    int  to[2];                /* estados destino de cada transición, -1 si no se usa    */
    char symbol[2];            /* símbolo de cada transición (EPSILON)                   */
    int  num_transitions;      /* número de transiciones salientes                       */
} nfa_state;

/* 
 * Un NFA completo, un arreglo dinámico de estados.
 */
typedef struct {
    nfa_state* states;      /* puntero a memoria de nfa_state    */
    int num_states;         /* número de estados guardados       */
    int capacity;           /* espacio reservado total, al
                             * menos tan grande como num_states  */
    int start;              /* estado inicial                    */
    int accept;             /* estado de aceptación              */
} nfa;

/* Convierte una expresión regular en notación postfija con concatenación
 * explícita a un NFA equivalente.
 */
nfa regex_to_nfa(regex r);

/* Simula el NFA sobre un input y regresa 1 si la
 * cadena es aceptada, 0 de lo contrario. 
 */
int match_nfa(nfa n, const char* str, int len);

/* 
 * Libera la memoria reservada por la construcción de cualquier NFA representado. 
 */
void free_nfa(nfa* n);

#endif
