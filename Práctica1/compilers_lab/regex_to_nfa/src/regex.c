#include "regex.h"
#include <stdlib.h>
#include <string.h>

/* 
 * Inicializa un regex con una capacidad inicial, 
 * estableciendo sus campos y reservando memoria.
 */
static void regex_init(regex* r, int capacity) {
    r->items = malloc(sizeof(regex_item) * capacity);
    r->size = 0;
    r->capacity = capacity;
}

/* 
 * Agrega un token al regex, actualizando sus campos y
 * reservando más memoria si es necesario.
 */
static void regex_push(regex* r, token_type type, char value) {
    if (r->size >= r->capacity) {
        r->capacity *= 2;
        r->items = realloc(r->items, sizeof(regex_item) * r->capacity);
    }
    r->items[r->size].type = type;
    r->items[r->size].value = value;
    r->size++;
}

/* 
 * Libera la memoria utilizada por un regex
 */
void free_regex(regex* r) {
    free(r->items);
    r->items = NULL;
    r->size = 0;
    r->capacity = 0;
}




/* 
 * Pasa un carácter crudo a su tipo de token correspondiente.
 */
static token_type type_of_char(char c) {
    switch (c) {
        case '|': return TOKEN_UNION;
        case '*': return TOKEN_STAR;
        case '+': return TOKEN_PLUS;
        case '?': return TOKEN_QUESTION;
        case '(': return TOKEN_PARENTOPEN;
        case ')': return TOKEN_PARENTCLOSE;
        default:  return TOKEN_CHAR;
    }
}

/* 
 * Crea un regex, recorre la cadena carácter por carácter
 * convirtiendolo en un token y agregándolo al regex.
 * Si se encuentra un '\' antes de un carácter, este último se toma como una literal.
 */

static regex tokenize(const char* input) {
    regex r;
    regex_init(&r, (int)strlen(input) + 1);
    for (int i = 0; input[i] != '\0'; i++) {
        char c = input[i];
        if (c == '\\' && input[i + 1] != '\0') {
            i++;
            regex_push(&r, TOKEN_CHAR, input[i]);
        } else {
            regex_push(&r, type_of_char(c), c);
        }
    }
    return r;
}




/* 
 * Determina si el tipo del token representa el final de un operando completo.
 */
static int ends_operand(token_type t) {
    return t == TOKEN_CHAR || t == TOKEN_PARENTCLOSE ||
           t == TOKEN_STAR || t == TOKEN_PLUS || t == TOKEN_QUESTION;
}

/* 
 * Determina si el tipo del token representa el inicio de un operando completo.
 */
static int starts_operand(token_type t) {
    return t == TOKEN_CHAR || t == TOKEN_PARENTOPEN;
}

/* 
 * Pasa de un regex con concatenación implícita a uno con concatenación explícita.
 * Agrega un token de concatenación entre dos operandos completos.
 */
static regex add_explicit_concat(regex* r) {
    regex output;
    regex_init(&output, r->size * 2 + 1);
    for (int i = 0; i < r->size; i++) {
        if (i > 0 && ends_operand(r->items[i - 1].type) &&
            starts_operand(r->items[i].type)) {
            regex_push(&output, TOKEN_CONCAT, '.');
        }
        regex_push(&output, r->items[i].type, r->items[i].value);
    }
    return output;
}




/* 
 * Determina la precedencia de un token según su tipo.
 */
static int precedence(token_type t) {
    switch (t) {
        case TOKEN_STAR:
        case TOKEN_PLUS:
        case TOKEN_QUESTION:
            return 3;
        case TOKEN_CONCAT:
            return 2;
        case TOKEN_UNION:
            return 1;
        default:
            return 0;
    }
}

/* 
 * Determina si un el tipo de token es un operador.
 */
static int is_operator(token_type t) {
    return t == TOKEN_CONCAT || t == TOKEN_UNION ||
           t == TOKEN_STAR || t == TOKEN_PLUS || t == TOKEN_QUESTION;
}

/* 
 * Convierte un regex con concatenación explícita a postfijo utilizando Shunting Yard.
 * Se crea el regex output y la pila de operadores, se recorre el input y 
 * se agregan los tokens al output o a la pila según su tipo y precedencia.
 * Finalmente, se vacía la pila restante al output y se libera la memoria de la pila.
 */
static regex to_postfix(regex* r) {
    regex output;
    regex_init(&output, r->size + 1);

    regex_item* op_stack = malloc(sizeof(regex_item) * (r->size + 1));
    int top = -1;

    for (int i = 0; i < r->size; i++) {
        regex_item current_token = r->items[i];

        if (current_token.type == TOKEN_CHAR) {
            regex_push(&output, current_token.type, current_token.value);

        } else if (current_token.type == TOKEN_PARENTOPEN) {
            op_stack[++top] = current_token;

        } else if (current_token.type == TOKEN_PARENTCLOSE) {
            while (top >= 0 && op_stack[top].type != TOKEN_PARENTOPEN) {
                regex_push(&output, op_stack[top].type, op_stack[top].value);
                top--;
            }
            if (top >= 0) top--;

        } else if (is_operator(current_token.type)) {
            while (top >= 0 && op_stack[top].type != TOKEN_PARENTOPEN &&
                   precedence(op_stack[top].type) >= precedence(current_token.type)) {
                regex_push(&output, op_stack[top].type, op_stack[top].value);
                top--;
            }
            op_stack[++top] = current_token;
        }
    }

    while (top >= 0) {
        regex_push(&output, op_stack[top].type, op_stack[top].value);
        top--;
    }

    free(op_stack);
    return output;
}




/* 
 * Convierte una cadena de expresión regular infija con concatenación
 * implícita a un regex postfijo con concatenación explícita.
 * Se tokeniza la cadena, se agrega la concatenación explícita y se convierte a postfijo,
 * liberando la memoria para cada regex intermedio.  
 */
regex parse_regex(const char* input) {
    regex tokens = tokenize(input);
    
    regex explicit_regex = add_explicit_concat(&tokens);
    free_regex(&tokens);

    regex postfix = to_postfix(&explicit_regex);
    free_regex(&explicit_regex);

    return postfix;
}