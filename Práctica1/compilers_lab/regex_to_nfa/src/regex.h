#ifndef REGEX_H
#define REGEX_H

/*
 * Tipos de token en la expresión regular (regex).
 */
typedef enum {
    TOKEN_CHAR,             /* 'a', 'b', '0',...  literal    */
    TOKEN_CONCAT,           /* '.'  concatenación            */
    TOKEN_UNION,            /* '|'  uno u otro               */
    TOKEN_STAR,             /* '*'  cero o más               */
    TOKEN_PLUS,             /* '+'  uno o más                */
    TOKEN_QUESTION,         /* '?'  uno o ninguno            */
    TOKEN_PARENTOPEN,       /* '('  abre paréntesis          */
    TOKEN_PARENTCLOSE       /* ')'  cierra paréntesis        */
} token_type;

/*
 * Un token de una expresión regular.
 */
typedef struct {
    token_type type;        /* tipo de token     */
    char value;             /* carácter crudo    */
} regex_item;

/**
 * Una expresión regular completa, un arreglo dinámico de tokens.
 */
typedef struct {
    regex_item* items;      /* puntero a memoria de regex_item    */
    int size;               /* numero de tokens guardados         */
    int capacity;           /* espacio reservado total, al
                             * menos tan grande como size         */
} regex;

/* 
 * Convierte una cadena de expresión regular infija con concatenación
 * implícita a un regex postfijo con concatenación explícita. 
 */
regex parse_regex(const char* input);

/* 
 * Libera la memoria reservada por la construcción de cualquier regex representado. 
 */
void free_regex(regex* r);

#endif