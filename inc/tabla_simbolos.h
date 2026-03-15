#pragma once

/**
 * @file tabla_simbolos.h
 * @brief Definición de las funciones públicas para la Tabla de Símbolos.
 *
 * Proporciona funciones para la manipulación de una Tabla de Símbolos.
 *
 * - Manipulación de la Tabla de Símbolos
 **/

#include "definiciones.h"

void tabla_simbolos_init();

void tabla_simobolos_terminate();

lexeme* intern_lexeme(lexeme* lexeme);

void tabla_simbolos_print();