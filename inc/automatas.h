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

/**********************
 *    ALFANUMÉRICO    *
 **********************/

typedef enum { anq_init, anq_ACCEPT, anq_COUNT_STATES } alfanumerico_states;

typedef enum {
    ana_letra,
    ana_digito,
    ana__,
    ana_COUNT_SYMBOLS
} alfanumerico_symbols;

static const short int alfnum_trans[anq_COUNT_STATES][ana_COUNT_SYMBOLS] = {
    // letra, digito
    [anq_init] = {anq_ACCEPT, -1},
    [anq_ACCEPT] = {anq_ACCEPT, anq_ACCEPT}};

/*****************
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
} numerico_states;

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
} numerico_symbols;

static const short int numerico_trans[nq_COUNT_STATES][na_COUNT_SYMBOLS] = {
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
    cq_begin,   ///< posible nuevo nivel de anidamiento, esperando '+'
    cq_push,        ///< Push en la pila. Nuevo nivel de anidamiento
    cq_end,     ///< posible fin de nivel de anidamiento, esperando '/'
    cq_pop,         ///< Pop en la pila, fin de un nivel de anidamiento
    cq_D,    ///< ACEPTA el atómico '/' (si no es el inicio de un comentario)
    cq_DE,    ///< ACEPTA el atómico '/=' (si no es el inicio de un comentario)
    cq_COUNT_STATES ///< Recuento de estados
} comentarios_states;

typedef enum {
    ca_SLASH,        ///< caracter '/'
    ca_STAR,         ///< caracter '*'
    ca_PLUS,         ///< caracter '+' para anidar comentarios
    ca_NEWLINE,      ///< rune '\n'
    ca_OTHER,        ///< cualquier otro caracter
    ca_E,            ///< caracter '=' 
    ca_COUNT_SYMBOLS ///< Recuento de símbolos
} comentarios_symbols;

static const short int comentarios_trans[cq_COUNT_STATES][ca_COUNT_SYMBOLS] = {
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
    [cq_pop]       = {cq_recursive, cq_recursive, cq_recursive,  cq_recursive, cq_recursive, cq_recursive}};

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
    aq_GGG,         ///< ACEPTA el atómico '>>>'
    aq_COUNT_STATES ///< Recuento de estados
} atomicos_states;

typedef enum {
    aa_STAR,         ///< caracter '*'
    aa_PLUS,         ///< caracter '+'
    aa_MINUS,        ///< caracter '-'
    aa_EQUAL,        ///< caracter '='
    aa_LOWER,        ///< caracter '<'
    aa_GREATER,      ///< caracter '>'
    aa_COUNT_SYMBOLS ///< Recuento de símbolos
} atomicos_symbols;

static const short int atomic_trans[aq_COUNT_STATES][aa_COUNT_SYMBOLS] = {
    //            *S        +P        -M          =E           <L         >G
    [aq_init] = {aq_S, aq_P,  aq_M,  aq_E,    aq_L,  aq_G},
    [aq_S]    = {-1,   -1,    -1,    aq_SE,   -1,    -1},
    [aq_SE]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_P]    = {-1,   aq_PP, -1,    aq_PE,   -1,    -1},
    [aq_PP]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_PE]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_M]    = {-1,   -1,    aq_MM, aq_ME,   -1,    -1},
    [aq_MM]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_ME]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_E]    = {-1,   -1,    -1,    aq_EE,   -1,    aq_EG},
    [aq_EE]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_EG]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_L]    = {-1,   -1,    -1,    aq_LE,   aq_LL, -1},
    [aq_LE]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_LLE]  = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_LL]   = {-1,   -1,    -1,    aq_LLE,  -1,    -1},
    [aq_G]    = {-1,   -1,    -1,    aq_GE,   -1,    aq_GG},
    [aq_GE]   = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_GGE]  = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_GGGE] = {-1,   -1,    -1,    -1,      -1,    -1},
    [aq_GG]   = {-1,   -1,    -1,    aq_GGE,  -1,    aq_GGG},
    [aq_GGG]  = {-1,   -1,    -1,    aq_GGGE, -1,    -1}};