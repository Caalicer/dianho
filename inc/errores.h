#pragma once

/**
 * @file errores.h
 * @brief Definición de las funciones públicas para el manejo de errores.
 *
 * Se usa un sistema de códigos y mensajes asociados, marcando la línea del
 * error, el tipo del mismo y un breve mensaje descriptivo.
 **/

#include <stdio.h>
#include "definiciones.h"

/**
 * @enum error_code
 * @brief Códigos de error para identificar diferentes tipos de errores.
 */
typedef enum {
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

typedef enum { MALFORMED_ATOMIC, MALFORMED_NUMBER, WARNING_COUNT } warning_code;

/**
 * @brief Mensajes de error correspondientes a cada código.
 */
static const char* errors[ERROR_COUNT] = {
    [UNKNOWN_SYMBOL] = "USYM: Símbolo no soportado.",
    [LEXEME_TOO_LONG] = "LTL: Lexema demasiado largo.",
    [NOT_INITIALIZED] = "INTERNO: uso de un componente no inicializado.",
    [MEMORY_ALLOCATION_ERROR] =
        "INTERNO: asignación de memoria con malloc fallida.",
    [UNFINISHED_BCOMMENT] = "UFC: Comentario de bloque no terminado.",
    [UNFINISHED_NCOMMENT] = "UFN: Comentario anidable no terminado.",
    [UNFINISHED_STRING] = "UFS: Cadena no terminada.",
    [DFA_INIT] = "INTERNO: DFA en estado inicial",
    [MALFORMED_BIN] = "MFB: Número binario mal formado.",
    [MALFORMED_EXP] = "MFE: Número con exponente mal formado.", 
    [FILE_OPEN_ERROR] = "FOPEN: No se pudo abrir el archivo.",
    [FILE_BEGIN] = "FBEGIN: No se puede retroceder, comienzo de fichero."};

static const char* warnings[WARNING_COUNT] = {
    [MALFORMED_ATOMIC] = "MFA: Posible operador o delimitador mal formado.\n\t "
                         "Falsos positivos como (( ))",
    [MALFORMED_NUMBER] = "MFN: Posible número mal formado.\n\t Falsos "
                         "positivos como 1+1 (1 + 1)"};

/**
 * @brief Emite un mensaje de error basado en el código de error proporcionado.
 * @param line El número de línea donde ocurrió el error.
 * @param code El código de error que identifica el tipo de error.
 */
static inline void emit_error(int line, error_code code) {
    if (code < 0 || code >= ERROR_COUNT || errors[code] == NULL) {
        print_err(C_RED, "\n[ERROR] Línea %d: error desconocido.\n\n", line);        return;
    }
    print_err(C_RED, "\n[ERROR] Línea %d %s\n\n", line, errors[code]);}

static inline void emit_warning(int line, warning_code code) {
    if (code < 0 || code >= WARNING_COUNT || warnings[code] == NULL) {
        print_err(C_ORANGE, "\n[WARNING] Línea %d: advertencia desconocida.\n\n", line);        return;
    }
    print_err(C_ORANGE, "\n[WARNING] Línea %d %s\n\n", line, warnings[code]);}