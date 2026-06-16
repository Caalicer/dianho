#pragma once

#include <stddef.h>

/// @brief Enumeración de los códigos de error del intérprete.
typedef enum { // To be Modified
    UNKNOWN_SYMBOL,
    LEXEME_TOO_LONG,
    NOT_INITIALIZED,
    MEMORY_ALLOCATION_ERROR,
    UNFINISHED_BCOMMENT,
    UNFINISHED_NCOMMENT,
    UNFINISHED_STRING,
    DFA_INIT,
    MALFORMED_BIN,
    MALFORMED_EXP,
    FILE_OPEN_ERROR,
    FILE_BEGIN,
    ERROR_COUNT
} error_code;

/// @brief Enumeración de los códigos de warning del intérprete.
typedef enum { // TO be modified
    WARN_VARIABLE_REDEFINED,
    WARN_IMPLICIT_CONVERSION,
    WARN_COUNT
} warning_code;

/// @brief Estructura que representa la ubicación de un error o warning en el
///        código fuente.
typedef struct {
    int line;
    int col;
    int len;
    char src_line[256];
} source_loc;

/// @brief Ubicación actual en el código fuente.
///        Actualizada por Flex en cada token. Leída internamente por emit_*.
extern source_loc current_loc;

/**
 * @brief Emite un warning e imprime. La ejecución continúa siempre.
 *
 * @param code     Código de warning
 * @param context  Información dinámica adicional, NULL si no aplica.
 * @param hint     Sobreescribe el hint por defecto, NULL para usar el interno.
 */
void emit_warning(error_code code, const char* context, const char* hint);

/**
 * @brief Emite un error e imprime. Devuelve 0 para que el caller pueda hacer
 *        return emit_error(...) y señalizar fallo al evaluador.
 *
 * @param code     Código de error
 * @param context  Información dinámica adicional, NULL si no aplica.
 * @param hint     Sobreescribe el hint por defecto, NULL para usar el interno.
 * @return int     Siempre devuelve 1 para indicar fallo
 */
int emit_error(error_code code, const char* context, const char* hint);

/**
 * @brief Emite un error irrecuperable, imprime y llama exit() para finalizar el
 *        intérprete.
 *
 * @param context  Información dinámica adicional, NULL si no aplica.
 * @param hint     Sobreescribe el hint por defecto, NULL para usar el interno.
 */
_Noreturn void emit_fatal(error_code code, const char* context,
                          const char* hint);