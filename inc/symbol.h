#pragma once

#include "function.h"
#include "value.h"

/// @brief Tipos de símbolos soportados
typedef enum {
    SYMBOL_VARIABLE, ///< Variable
    SYMBOL_CONSTANT, ///< Constante predefinida
    SYMBOL_FUNCTION, ///< Función
    SYMBOL_LIBRARY   ///< Librería cargada
} symbol_type;

/// @brief Estructura que representa un símbolo en la tabla de símbolos.
typedef struct {
    symbol_type type;
    union {
        value val;        ///< Valor de la variable o constante
        function* fn;     ///< Puntero a la función
        void* lib_handle; ///< Handle de la librería cargada
    } data;
} symbol;