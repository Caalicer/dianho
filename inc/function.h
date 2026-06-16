#pragma once

/**
 * @file function.h
 * @brief Definición de la estructura para representar funciones del intérprete.
 */

#include "ast.h"
#include <stddef.h>

/// @brief Tipos de funciones soportadas
typedef enum {
    FN_USER,  ///< Función definida por el usuario
    FN_NATIVE ///< Función cargada externa
} function_type;

typedef struct eval_env {
    const char** params;
    const value* args;
    size_t argc;
} eval_env;

typedef struct function {
    function_type type; ///< Tag del tipo de función
    union {
        struct {
            char** param_names; ///< Nombres de los parámetros
            size_t arity;       ///< Número de parámetros
            ast_node* body;     ///< Cuerpo de la función
        } user;
        struct {
            void* sym; ///< Símbolo de la función cargada dinámicamente.
        } native;
    } data;
} function;

int function_init_user(function* fn, char** params, size_t arity,
                       ast_node* body);

int function_init_native(void* handler, void* sym);

int function_call(const function* fn, eval_env* env, value* out);

