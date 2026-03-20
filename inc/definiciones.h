#pragma once
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_TOKEN_LENGTH 2048

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
static inline char* fragments_to_string(const fragments* frags){
    
    if (!frags) return NULL;
    
    size_t total = frags->len1 + frags->len2;
    char* str = malloc(total + 1);
    if (!str) return NULL;
    
    memcpy(str, frags->fr1, frags->len1);
    if (frags->fr2 != NULL)
        memcpy(str + frags->len1, frags->fr2, frags->len2);
    
    str[total] = '\0';
    return str;

}

static inline void print_lexeme(lexeme e) {
    printf("\t <%d, %s>\n", e.lexical_token, e.lexeme);
}

/**
 * @brief Enumeración de los componentes léxicos.
 */
typedef enum Token {

    ID = 300,

    // Keywords presentes en `regression.d`
    IMPORT,
    DOUBLE,
    VOID,
    INT,
    WHILE,
    FOREACH,
    CAST,

    // Literales
    INTEGER_LITERAL,
    BINARY_LITERAL,
    FLOAT_LITERAL,
    EXPONENTIAL_LITERAL,
    STRING_LITERAL,
    
    // ATÓMICOS
    DDOT,        ///< atómico '..'
    DDDOT,       ///< atómico '...'
    DE,          ///< atómico '/='
    SE,          ///< atómico '*='
    PP,          ///< atómico '++'
    PE,          ///< atómico '+='
    MM,          ///< atómico '--'
    ME,          ///< atómico '-='
    EE,          ///< atómico '=='
    EG,          ///< atómico '=>'
    LE,          ///< atómico '<='
    LLE,         ///< atómico '<<='
    LL,          ///< atómico '<<'
    GE,          ///< atómico '>='
    GGE,         ///< atómico '>>='
    GGGE,        ///< atómico '>>>='
    GG,          ///< atómico '>>'
    GGG,         ///< atómico '>>>'

    TOKEN_ERROR = 500, ///< Componente léxico asociado a un fallo

} Token;

/*********** KEYWORDS ***********/

// ABSTRACT
// ALIAS
// ALIGN
// ASM
// ASSERT
// AUTO
// BODY
// BOOL
// BREAK
// BYTE
// CASE
// CAST
// CATCH
// CDOUBLE
// CENT
// CFLOAT
// CHAR
// CLASS
// CONST
// CONTINUE
// CREAL
// DCHAR
// DEBUG
// DEFAULT
// DELEGATE
// DELETE
// DEPRECATED
// DO
// DOUBLE
// ELSE
// ENUM
// EXPORT
// EXTERN
// FALSE
// FINAL
// FINALLY
// FLOAT
// FOR
// FOREACH
// FOREACH_REVERSE
// FUNCTION
// GOTO
// IDOUBLE
// IF
// IFLOAT
// IMMUTABLE
// IMPORT
// IN
// INOUT
// INT
// INTERFACE
// INVARIANT
// IREAL
// IS
// LAZY
// LONG
// MACRO
// MIXIN
// MODULE
// NEW
// NOTHROW
// NULL
// OUT
// OVERRIDE
// PACKAGE
// PRAGMA
// PRIVATE
// PROTECTED
// PUBLIC
// PURE
// REAL
// REF
// RETURN
// SCOPE
// SHARED
// SHORT
// STATIC
// STRUCT
// SUPER
// SWITCH
// SYNCHRONIZED
// TEMPLATE
// THIS
// THROW
// TRUE
// TRY
// TYPEID
// TYPEOF
// UBYTE
// UCENT
// UINT
// ULONG
// UNION
// UNITTEST
// USHORT
// VERSION
// VOID
// WCHAR
// WHILE
// WITH
// __FILE__
// __FILE_FULL_PATH__
// __FUNCTION__
// __LINE__
// __MODULE__
// __PRETTY_FUNCTION__
// __GSHARED
// __PARAMETERS
// __RVALUE
// __TRAITS
// __VECTOR