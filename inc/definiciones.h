#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKEN_LENGTH 1024

/**
 * @brief Estructura que representa un lexema en el analizador léxico.
 */
typedef struct {
    int lexical_token;
    char* lexeme;
} lexeme;

/**
 * @enum fragments Una string fragmentada en 2 partes
 * @brief Almacena un string partido en dos fragmentos
 */
typedef struct {
    const char* fr1; ///< Fragmento 1 del string
    size_t len1;     ///< Longitud del fragmento
    const char* fr2; ///< Fragmento 2 del string. Puede ser NULL
    size_t len2;     ///< Longitud del fragmento 2. 0 si fr2 es NULL
} fragments;

/**
 * @brief Reserva memoria para un nuevo string concatenando los fragmentos
 *
 * @param frags fragmentos a concatenar
 * @return char* Puntero al nuevo string concatenado.
 *               El caller es responsable de liberar la memoria.
 */
static inline char* fragments_to_string(const fragments* frags) {

    if (!frags)
        return NULL;

    size_t total = frags->len1 + frags->len2;
    char* str = malloc(total + 1);
    if (!str)
        return NULL;

    memcpy(str, frags->fr1, frags->len1);
    if (frags->fr2 != NULL)
        memcpy(str + frags->len1, frags->fr2, frags->len2);

    str[total] = '\0';
    return str;
}

/**
 * @brief Enumeración de los componentes léxicos.
 */
// typedef enum Token {

//     // IDENTIFICADORES
//     ID = 300,

//     // ATÓMICOS
//     POW,     ///< atómico '**'
//     DE,      ///< atómico '/='
//     SE,      ///< atómico '*='
//     PE,      ///< atómico '+='
//     ME,      ///< atómico '-='
//     EE,      ///< atómico '=='
//     LE,      ///< atómico '<='
//     GE,      ///< atómico '>='
//     NE,      ///< atómico '!='
//     AND,     ///< atómico '&&'
//     OR,      ///< atómico '||'
//     LSHIFT,  ///< atómico '<<'
//     RSHIFT,  ///< atómico '>>'
//     NEWLINE, ///< atómico '\n'

//     // LITERALES
//     INTEGER_LITERAL,
//     BOOLEAN_LITERAL,
//     FLOAT_LITERAL,
//     EXPONENTIAL_LITERAL,
//     STRING_LITERAL,

//     // KEYWORDS

//     TOKEN_ERROR = 999 ///< Componente léxico asociado a un fallo

// } Token;

// COLORES
#define C_RED "\x1b[31m"
#define C_WHITE "\x1b[97m"
#define C_UGREEN "\x1b[4;32m"
#define C_ORANGE "\x1b[38;5;208m"
#define C_BYELLOW "\x1b[38;5;226m" // amarillo intenso
#define C_IBLUE "\x1b[38;5;33m"
#define C_LBLUE "\x1b[38;5;117m" // azul claro
#define C_PINK "\x1b[38;5;205m"       // rosa
#define C_MAGENTA "\x1b[35m"

// ESTILOS
#define C_RESET "\x1b[0m"
#define C_BOLD "\x1b[1m"
#define C_DIM "\x1b[2m"
#define C_UNDERLINE "\x1b[4m"
#define C_BLINK "\x1b[5m"
#define C_REVERSE "\x1b[7m"
#define C_HIDDEN "\x1b[8m"

#define print_err(COLOR, FMT, ...)                                             \
    fprintf(stderr, COLOR FMT C_RESET, __VA_ARGS__)

// static inline const char* lexeme_color(uint16_t tipo) {

//     if (tipo == ID)
//         return C_BYELLOW;
//     else if (tipo <= GGG)
//         return C_MAGENTA;
//     else if (tipo <= EXPONENTIAL_LITERAL)
//         return C_IBLUE;
//     else if (tipo == STRING_LITERAL)
//         return C_LBLUE;
//     else if (tipo >= ABSTRACT && tipo <= __VECTOR)
//         return C_PINK;
//     else
//         return C_RED;
// }

// static inline void print_lexeme(lexeme e) {
//     const char* tok_color = lexeme_color(e.lexical_token);

//     printf("\t" C_WHITE "<" // apertura en blanco
//            "%s%d" C_RESET    // token con color y reset
//            C_WHITE ", " C_UGREEN "%s" C_RESET // lexeme verde y reset
//            C_WHITE ">" C_RESET "\n", // cierre en blanco + reset
//            tok_color, e.lexical_token, e.lexeme);
// }