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

/**
 * @brief Inicializa la Tabla de Símbolos.
 *
 * Se cargan inicialmente los keywords y se reserva la memoria necesaria.
 *
 * En caso de ya existir una Tabla de Símbolos previa, no hace nada.
 *
 */
void symtab_init();

/**
 * @brief Destruye la Tabla de Símbolos.
 *
 * Libera toda la memoria asociad a la Tabla de Símbolos, borrando su contenido.
 *
 * En caso de no existir una Tabla de Símbolos previa, no hace nada.
 *
 */
void symtab_terminate();

/**
 * @brief Interna un lexema en la Tabla de Símbolos. Busca el elemento en la
 * Tabla de Símbolos. Si existe, devuelve un puntero al elemento existente. Si
 * no existe, lo inserta y devuelve un puntero al nuevo elemento.
 *
 * @param lexeme Lexema a internar.
 * @return lexeme* Puntero al elemento internado en la Tabla de Símbolos.
 * @retval NULL Hubo un error en la inserción.
 */
lexeme* symtab_intern(lexeme* lexeme);

/**
 * @brief Imprime el contenido completo de la Tabla de Símbolos.
 * 
 */
void symtab_print();