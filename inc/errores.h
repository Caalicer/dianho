#pragma once

/**
 * @file errores.h
 * @brief Definición de las funciones públicas para el manejo de errores.
 *
 * Se usa un sistema de códigos y mensajes asociados, marcando la línea del
 * error, el tipo del mismo y un breve mensaje descriptivo.
 **/

#include <stdio.h>

/**
 * @enum error_code
 * @brief Códigos de error para identificar diferentes tipos de errores.
 */
typedef enum { ERROR_1, ERROR_2, ERROR_3, ERROR_COUNT } error_code;

/**
 * @brief Mensajes de error correspondientes a cada código.
 */
static const char* errors[ERROR_COUNT] = {
    [ERROR_1] = "1: Descripción del error 1.",
    [ERROR_2] = "2: Descripción del error 2.",
    [ERROR_3] = "3: Descripción del error 3."};

/**
 * @brief Emite un mensaje de error basado en el código de error proporcionado.
 * @param line El número de línea donde ocurrió el error.
 * @param code El código de error que identifica el tipo de error.
 */
static inline void emit_error(int line, error_code code) {
    if (code < 0 || code >= ERROR_COUNT || errors[code] == NULL) {
        fprintf(stderr, "\nLínea %d: error desconocido.\n\n", line);
        return;
    }
    fprintf(stderr, "\nLínea %d %s\n\n", line, errors[code]);
}