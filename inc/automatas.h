#pragma once
/**
 * @file automatas.h
 * @brief Implementación de los autómatas usados por el lexer:
 * - Alfanumérico: Identificadores y palabras reservadas.
 * - Numérico: Literales enteras, punto flotante y sus variantes con puntos.
 * - Comentarios: de bloque, línea y anidados (pila externa)
 * - Strings: únicamente DoubleQuotedStrings.
 * - Atomicos: Operadores y puntuación.
 *
 * Notación para estados y símbolos:
 *
 * - Estados: (a)q_[n]:
 *  - a: abreviatura del automata.
 *  - q: indica que es un estado.
 *  - n: numero de estado, o nombre si tiene semántica (como inicial).
 *       Si es de aceptación se marca con mayúsculas.
 * - Símbolos: a_(n):
 *  - a: indica que es del (a)lfabeto del automata.
 *  - n: nombre del símbolo.
 */

#include <ctype.h>
#include <stdint.h>
#include "definiciones.h"
#include "errores.h"

/**
 * @enum char_type Diferentes tipos de caracteres
 * @brief Agrupa los caracteres en tipos gestionables por un mismo handler
 *
 * > [!CAUTION] Añadir entradas
 * > Los dfas tiene que ir al principio para poder usarlos como índices.
 * > Inmediantamente después siempre debe ir DFA_COUNT.
 */
typedef enum {
    ALPHANUMERIC,
    NUMERIC,
    COMMENTS,
    ATOMICS,
    STRINGS,
    DFA_COUNT, ///< Recuento de tipos. Importante que los DFA sea
    WHITESPACE,
    D_EOF,
    UNKNOWN
} implemented_dfa;

/**********************
 *    ALFANUMÉRICO    *
 **********************/

/// Mappers para cada autómata,

typedef enum { anq_init, anq_ACCEPT, anq_COUNT_STATES } alphanumeric_states;

typedef enum {
    ana_letra,
    ana_digito,
    ana__,
    ana_COUNT_SYMBOLS
} alphanumeric_symbols;

static const short int alphanum_trans[anq_COUNT_STATES][ana_COUNT_SYMBOLS] = {
    // letra, digito, ana__
    [anq_init]   = {anq_ACCEPT, -1,         anq_ACCEPT},
    [anq_ACCEPT] = {anq_ACCEPT, anq_ACCEPT, anq_ACCEPT}};

static uint64_t alphanum_accpeting_bitmap = (1ULL << anq_ACCEPT);

static short int alphanumeric_token[anq_COUNT_STATES] = {
    [anq_init] = DFA_INIT,
    [anq_ACCEPT] = ID};

static char alphanumeric_map[256];

void alphanum_mapper(){
    
    for (int i = 0; i < 256; i++) {
        if (isalpha(i)) alphanumeric_map[i] = ana_letra;
        else if (isdigit(i)) alphanumeric_map[i] = ana_digito;
        else alphanumeric_map[i] = -1; // Símbolo no mapeado
    }
    alphanumeric_map['_'] = ana__;
}

/******************
 *    NUMÉRICO    *
 ******************/

typedef enum {
    nq_init,        ///< Inicial
    nq_INTB,        ///< ACEPTA enteros, a medio camino de identificar binario
    nq_bin,         ///< formando binario como '0b'
    nq_BIN,         ///< ACEPTA binario
    nq_INT,         ///< ACEPTA enteros
    nq_FLOATD,      ///< ACEPTA floats que terminan en . (Dot) como '1.'
    nq_FLOATM,      ///< ACEPTA floats con mantisa como '1.0'
    nq_exp,         ///< formando flotante con exponente, como '1e'
    nq_FLOATE,      ///< ACEPTA flotantes con exponente, como '1e10'
    nq_DOT,         ///< ACEPTA el atómico '.'
    nq_DDOT,        ///< ACEPTA el atómico '..'
    nq_DDDOT,       ///< ACEPTA el atómico '...'
    nq_COUNT_STATES ///< Recuento de estados
} numeric_states;

typedef enum {
    na_0,            ///< número 0
    na_1,            ///< número 1
    na_num,          ///< números del 2 al 9
    na_b,            ///< letra 'b' para binarios
    na__,            ///< guion bajo
    na_dot,          ///< punto
    na_e,            ///< letra 'e' para exponentes
    na_E,            ///< letra 'E' para exponentes
    na_PLUS,         ///< signo '+' para exponentes
    na_MINUS,        ///< signo '-' para exponentes
    na_COUNT_SYMBOLS ///< Recuento de símbolos
} numeric_symbols;

static const short int numeric_trans[nq_COUNT_STATES][na_COUNT_SYMBOLS] = {
    //              0,             1,             num,           b,          _,             .,             e,          E,          +,          -
    [nq_init]   = {nq_INTB,   nq_INT,    nq_INT,    -1,     -1,        nq_DOT,    -1,     -1,     -1,        -1},
    [nq_INTB]   = {nq_INT,    nq_INT,    nq_INT,    nq_bin, nq_INT,    nq_FLOATD, nq_exp, nq_exp, -1,        -1},
    [nq_bin]    = {nq_BIN,    nq_BIN,    -1,        -1,     nq_BIN,    -1,        -1,     -1,     -1,        -1},
    [nq_BIN]    = {nq_BIN,    nq_BIN,    -1,        -1,     nq_BIN,    -1,        -1,     -1,     -1,        -1},
    [nq_INT]    = {nq_INT,    nq_INT,    nq_INT,    -1,     nq_INT,    nq_FLOATD, nq_exp, nq_exp, -1,        -1},
    [nq_FLOATD] = {nq_FLOATM, nq_FLOATM, nq_FLOATM, -1,     -1,        -1,        -1,     -1,     -1,        -1},
    [nq_FLOATM] = {nq_FLOATM, nq_FLOATM, nq_FLOATM, -1,     nq_FLOATM, -1,        nq_exp, nq_exp, -1,        -1},
    [nq_exp]    = {nq_FLOATE, nq_FLOATE, nq_FLOATE, -1,     nq_FLOATE, -1,        -1,     -1,     nq_FLOATE, nq_FLOATE},
    [nq_FLOATE] = {nq_FLOATE, nq_FLOATE, nq_FLOATE, -1,     nq_FLOATE, -1,        -1,     -1,     -1,        -1},
    [nq_DOT]    = {nq_FLOATM, nq_FLOATM, nq_FLOATM, -1,     -1,        nq_DDOT,   -1,     -1,     -1,        -1},
    [nq_DDOT]   = {-1,        -1,        -1,        -1,     -1,        nq_DDDOT,  -1,     -1,     -1,        -1},
    [nq_DDDOT]  = {-1,        -1,        -1,        -1,     -1,        -1,        -1,     -1,     -1,        -1}};

static uint64_t numeric_accpeting_bitmap =
    (1ULL << nq_INTB)   | (1ULL << nq_BIN)    | (1ULL << nq_INT)    |
    (1ULL << nq_FLOATD) | (1ULL << nq_FLOATM) | (1ULL << nq_FLOATE) |
    (1ULL << nq_DOT)    | (1ULL << nq_DDOT)   | (1ULL << nq_DDDOT);

static short int numeric_token[nq_COUNT_STATES] = {
    [nq_init] = DFA_INIT,
    [nq_INTB] = INTEGER_LITERAL,
    [nq_bin] = MALFORMED_BIN,
    [nq_BIN] = BINARY_LITERAL,
    [nq_INT] = INTEGER_LITERAL,
    [nq_FLOATD] = FLOAT_LITERAL,
    [nq_FLOATM] = FLOAT_LITERAL,
    [nq_exp] = MALFORMED_EXP,
    [nq_FLOATE] = EXPONENTIAL_LITERAL,
    [nq_DOT] = '.',
    [nq_DDOT] = DDOT,
    [nq_DDDOT] = DDDOT,
};
static char numeric_map[256];

void numeric_mapper(){

    for(int i = 0; i<256; i++){
        if(isdigit(i)) numeric_map[i] = na_num;
        else numeric_map[i] = -1; 
    }
    numeric_map['0'] = na_0; numeric_map['_'] = na__; numeric_map['.'] = na_dot;
    numeric_map['1'] = na_1; numeric_map['e'] = na_e; numeric_map['+'] = na_PLUS;
    numeric_map['b'] = na_b; numeric_map['E'] = na_E; numeric_map['-'] = na_MINUS; 
}

/********************
 *    COMENTARIOS    *
 *********************/

typedef enum {
    cq_init,        ///< Inicial
    cq_selector,    ///< formando un comentario de los tres tipos
    cq_line,        ///< formando comentario de línea
    cq_LINE,        ///< ACEPTA comentario de línea
    cq_block,       ///< formando comentario de bloque
    cq_block2,      ///< formando comentario de bloque, esperando cierre '/**'
    cq_BLOCK,       ///< ACEPTA comentario de bloque
    cq_recursive,   ///< formando comentario anidado
    cq_begin,       ///< posible nuevo nivel de anidamiento, esperando '+'
    cq_push,        ///< Push en la pila. Nuevo nivel de anidamiento
    cq_end,         ///< posible fin de nivel de anidamiento, esperando '/'
    cq_pop,         ///< Pop en la pila, fin de un nivel de anidamiento
    cq_D,           ///< ACEPTA el atómico '/' (si no es el inicio de un comentario)
    cq_DE,          ///< ACEPTA el atómico '/=' (si no es el inicio de un comentario)
    cq_COUNT_STATES ///< Recuento de estados
} comments_states;

typedef enum {
    ca_SLASH,        ///< caracter '/'
    ca_STAR,         ///< caracter '*'
    ca_PLUS,         ///< caracter '+' para anidar comentarios
    ca_NEWLINE,      ///< rune '\n'
    ca_OTHER,        ///< cualquier otro caracter
    ca_E,            ///< caracter '='
    ca_COUNT_SYMBOLS ///< Recuento de símbolos
} comments_symbols;

static const short int comments_trans[cq_COUNT_STATES][ca_COUNT_SYMBOLS] = {
    //                 /                 *                 +                 \n                 OTHER             =
    [cq_init]      = {cq_selector,  -1,           -1,            -1,           -1,           -1},
    [cq_selector]  = {cq_line,      cq_block,     cq_recursive,  -1,           cq_D,         cq_DE},
    [cq_line]      = {cq_line,      cq_line,      cq_line,       cq_LINE,      cq_line,      cq_line},
    [cq_LINE]      = {-1,           -1,           -1,            -1,           -1,           -1},
    [cq_block]     = {cq_block,     cq_block2,    cq_block,      cq_block,     cq_block,     cq_block},
    [cq_block2]    = {cq_BLOCK,     cq_block2,    cq_block,      cq_block,     cq_block,     cq_block},
    [cq_BLOCK]     = {-1,           -1,           -1,            -1,           -1,           -1},
    [cq_recursive] = {cq_begin,     cq_recursive, cq_end,        cq_recursive, cq_recursive, cq_recursive},
    [cq_begin]     = {cq_recursive, cq_recursive, cq_push,       cq_recursive, cq_recursive, cq_recursive},
    [cq_push]      = {cq_recursive, cq_recursive, cq_recursive,  cq_recursive, cq_recursive, cq_recursive},
    [cq_end]       = {cq_pop,       cq_recursive, cq_recursive,  cq_recursive, cq_recursive, cq_recursive},
    [cq_pop]       = {cq_recursive, cq_recursive, cq_recursive,  cq_recursive, cq_recursive, cq_recursive},
    [cq_D]         = {-1,           -1,           -1,            -1,           -1,           -1},
    [cq_DE]        = {-1,           -1,           -1,            -1,           -1,           -1}};

static uint64_t comments_accpeting_bitmap = 
    (1ULL << cq_LINE) | (1ULL << cq_BLOCK) | (1ULL << cq_D) | (1ULL << cq_DE);

static short int comments_token[cq_COUNT_STATES] = {
    [cq_init] = DFA_INIT,
    [cq_selector] = DFA_INIT,
    [cq_line] = UNFINISHED_STRING,
    [cq_LINE] = -2,  // No emite token y se ignora
    [cq_block] = UNFINISHED_BCOMMENT,
    [cq_block2] = UNFINISHED_BCOMMENT,
    [cq_BLOCK] = -2, // -2 porque EOF es -1
    [cq_recursive] = UNFINISHED_NCOMMENT,
    [cq_begin] = UNFINISHED_NCOMMENT,
    [cq_push] = UNFINISHED_NCOMMENT,
    [cq_end] = UNFINISHED_NCOMMENT,
    [cq_pop] = UNFINISHED_NCOMMENT,
    [cq_D] = '/',
    [cq_DE] = DE};

static char comments_map[256];

void comments_mapper(){

    for(int i = 0; i<256; i++){
        comments_map[i] = ca_OTHER; 
    }
    comments_map['/']  = ca_SLASH;   comments_map['*'] = ca_STAR;
    comments_map['+'] = ca_PLUS;     comments_map['='] = ca_E;
    comments_map['\n'] = ca_NEWLINE; 
}

/****************
 *    STRINGS    *
 *****************/

typedef enum {
    sq_init,        ///< Inicial
    sq_body,        ///< Cuerpo del string
    sq_escape,      ///< Escapamos caracteres con '\'
    sq_ACCEPT,      ///< ACEPTA el string
    sq_COUNT_STATES ///< Recuento de estados
} strings_states;

typedef enum {
    sa_QUOTE,        ///< caracter '"'
    sa_BACKSLASH,    ///< caracter '\'
    sa_OTHER,        ///< cualquier otro caracter
    sa_COUNT_SYMBOLS ///< Recuento de símbolos
} strings_symbols;

static const short int strings_trans[sq_COUNT_STATES][sa_COUNT_SYMBOLS] = {
    //              "              \              OTHER
    [sq_init]   = {sq_body,   -1,        -1},
    [sq_body]   = {sq_ACCEPT, sq_escape, sq_body},
    [sq_escape] = {sq_body,   sq_body,   sq_body},
    [sq_ACCEPT] = {-1,        -1,        -1}};

static uint64_t strings_accpeting_bitmap = (1ULL << sq_ACCEPT);

static short int strings_token[sq_COUNT_STATES] = {
    [sq_init]   = DFA_INIT,
    [sq_body]   = UNFINISHED_STRING,
    [sq_escape] = UNFINISHED_STRING,
    [sq_ACCEPT] = STRING_LITERAL};

static char strings_map[256];

void strings_mapper(){
    
    for(int i = 0; i<256; i++){
        strings_map[i] = sa_OTHER; 
    }
    strings_map['"'] = sa_QUOTE;
    strings_map['\\'] = sa_BACKSLASH;
}

/*****************
 *    ATÓMICOS    *
 ******************/

typedef enum {
    aq_init,        ///< Inicial
    aq_S,           ///< ACEPTA el atómico '*'
    aq_SE,          ///< ACEPTA el atómico '*='
    aq_P,           ///< ACEPTA el atómico '+'
    aq_PP,          ///< ACEPTA el atómico '++'
    aq_PE,          ///< ACEPTA el atómico '+='
    aq_M,           ///< ACEPTA el atómico '-'
    aq_MM,          ///< ACEPTA el atómico '--'
    aq_ME,          ///< ACEPTA el atómico '-='
    aq_E,           ///< ACEPTA el atómico '='
    aq_EE,          ///< ACEPTA el atómico '=='
    aq_EG,          ///< ACEPTA el atómico '=>'
    aq_L,           ///< ACEPTA el atómico '<'
    aq_LE,          ///< ACEPTA el atómico '<='
    aq_LLE,         ///< ACEPTA el atómico '<<='
    aq_LL,          ///< ACEPTA el atómico '<<'
    aq_G,           ///< ACEPTA el atómico '>'
    aq_GE,          ///< ACEPTA el atómico '>='
    aq_GGE,         ///< ACEPTA el atómico '>>='
    aq_GGGE,        ///< ACEPTA el atómico '>>>='
    aq_GG,          ///< ACEPTA el atómico '>>'
    aq_GGG,         ///< ACEPTA el atómico '>>>'j
    aq_LP,          ///< ACEPTA el atómico '('
    aq_RP,          ///< ACEPTA el atómico ')'
    aq_LB,          ///< ACEPTA el atómico '{'
    aq_RB,          ///< ACEPTA el atómico '}'
    aq_LBR,         ///< ACEPTA el atómico '['
    aq_RBR,         ///< ACEPTA el atómico ']'
    aq_SEMI,        ///< ACEPTA el atómico ';'
    aq_COMMA,       ///< ACEPTA el atómico ','
    aq_COUNT_STATES ///< Recuento de estados
} atomics_states;

typedef enum {
    aa_STAR,         ///< caracter '*'
    aa_PLUS,         ///< caracter '+'
    aa_MINUS,        ///< caracter '-'
    aa_EQUAL,        ///< caracter '='
    aa_LOWER,        ///< caracter '<'
    aa_GREATER,      ///< caracter '>'
    aa_LPAREN,       ///< caracter '('
    aa_RPAREN,       ///< caracter ')'
    aa_LBRACE,       ///< caracter '{'
    aa_RBRACE,       ///< caracter '}'
    aa_LBRACKET,     ///< caracter '['
    aa_RBRACKET,     ///< caracter ']'
    aa_SEMI,        ///< caracter ';'
    aa_COMMA,       ///< caracter ','
    aa_COUNT_SYMBOLS ///< Recuento de símbolos
} atomics_symbols;

static const short int atomics_trans[aq_COUNT_STATES][aa_COUNT_SYMBOLS] = {
    //            *S        +P        -M          =E           <L         >G        (LP        )RP        {LB        }RB        [LBR        ]RBR      ;SEMI      ,COMMA
    [aq_init] = {aq_S, aq_P,  aq_M,  aq_E,    aq_L,  aq_G,   aq_LP, aq_RP, aq_LB, aq_RB, aq_LBR, aq_RBR, aq_SEMI, aq_COMMA},
    [aq_S]    = {-1,   -1,    -1,    aq_SE,   -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_SE]   = {-1,   -1,    -1,    -1,      -1,    -1 -1,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_P]    = {-1,   aq_PP, -1,    aq_PE,   -1,    -1 -1,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_PP]   = {-1,   -1,    -1,    -1,      -1,    -1 -1,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_PE]   = {-1,   -1,    -1,    -1,      -1,    -1 -1,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_M]    = {-1,   -1,    aq_MM, aq_ME,   -1,    -1 -1,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_MM]   = {-1,   -1,    -1,    -1,      -1,    -1 -1,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_ME]   = {-1,   -1,    -1,    -1,      -1,    -1 -1,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_E]    = {-1,   -1,    -1,    aq_EE,   -1,    aq_EG,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_EE]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_EG]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_L]    = {-1,   -1,    -1,    aq_LE,   aq_LL, -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_LE]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_LLE]  = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_LL]   = {-1,   -1,    -1,    aq_LLE,  -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_G]    = {-1,   -1,    -1,    aq_GE,   -1,    aq_GG,  -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_GE]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_GGE]  = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_GGGE] = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_GG]   = {-1,   -1,    -1,    aq_GGE,  -1,    aq_GGG, -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_GGG]  = {-1,   -1,    -1,    aq_GGGE, -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_LP]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_RP]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_LB]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_RB]   = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_LBR]  = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_RBR]  = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_SEMI] = {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1},
    [aq_COMMA]= {-1,   -1,    -1,    -1,      -1,    -1,     -1,    -1,    -1,    -1,    -1,     -1,     -1,      -1}};

static uint64_t atomics_accpeting_bitmap =  (1ULL << aq_S)  |
    (1ULL << aq_SE)  | (1ULL << aq_P)    |   (1ULL << aq_PP) | (1ULL << aq_PE) |
    (1ULL << aq_M)   | (1ULL << aq_MM)   |   (1ULL << aq_ME) | (1ULL << aq_E)  |
    (1ULL << aq_EE)  | (1ULL << aq_EG)   |   (1ULL << aq_L)  | (1ULL << aq_LE) |
    (1ULL << aq_LLE) | (1ULL << aq_LL)   |   (1ULL << aq_G)  | (1ULL << aq_GE) |
    (1ULL << aq_GGE) | (1ULL << aq_GGGE) |   (1ULL << aq_GG) | (1ULL << aq_GGG)|
    (1ULL << aq_LP)  | (1ULL << aq_RP)   |   (1ULL << aq_LB) | (1ULL << aq_RB) |
    (1ULL << aq_LBR) | (1ULL << aq_RBR)  |   (1ULL << aq_SEMI)| (1ULL << aq_COMMA);

static short int atomics_token[aq_COUNT_STATES] = {
    [aq_init] = DFA_INIT,
    [aq_S] = '*',   [aq_SE] = SE,
    [aq_P] = '+',   [aq_PP] = PP,   [aq_PE] = PE,
    [aq_M] = '-',   [aq_MM] = MM,   [aq_ME] = ME,
    [aq_E] = '=',   [aq_EE] = EE,   [aq_EG] = EG,
    [aq_L] = '<',   [aq_LE] = LE,   [aq_LLE] = LLE, [aq_LL] = LL,
    [aq_G] = '>',   [aq_GE] = GE,   [aq_GGE] = GGE, [aq_GGGE] = GGGE, [aq_GG] = GG, [aq_GGG] = GGG,
    [aq_LP] = '(',  [aq_RP] = ')',
    [aq_LB] = '{',  [aq_RB] = '}',
    [aq_LBR] = '[', [aq_RBR] = ']',
    [aq_SEMI] = ';', [aq_COMMA] = ',',
};

static char atomics_map[256];

void atomics_mapper(){

    for(int i = 0; i<256; i++){
        atomics_map[i] = -1; 
    }
    atomics_map['*'] = aa_STAR;  atomics_map['+'] = aa_PLUS;
    atomics_map['-'] = aa_MINUS; atomics_map['='] = aa_EQUAL;
    atomics_map['<'] = aa_LOWER; atomics_map['>'] = aa_GREATER;
    atomics_map['('] = aa_LPAREN; atomics_map[')'] = aa_RPAREN;
    atomics_map['{'] = aa_LBRACE; atomics_map['}'] = aa_RBRACE;
    atomics_map['['] = aa_LBRACKET; atomics_map[']'] = aa_RBRACKET;
    atomics_map[';'] = aa_SEMI; atomics_map[','] = aa_COMMA;
}