#pragma once

/**
 * @file tabla_simbolos.h
 * @brief Definición de las funciones públicas para la Tabla de Símbolos.
 *
 * Proporciona funciones para la manipulación de una Tabla de Símbolos a través
 * de parejas key (char*) value (symbol*).
 *
 **/

#include "symbol.h"

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
 * @brief Inserta en la Tabla de Símbolos un nuevo símbolo asociado a un nombre.
 *
 * En caso de existir un símbolo con el mismo nombre (colisión):
 * - Constante, librería o una función cargada dinámicamente: fallo.
 * - Variable o función definida por el usuario: se reemplaza el valor.
 *
 * @param key Nombre por el que se guardará el símbolo en la Tabla de Símbolos.
 * @param sym Puntero al símbolo a insertar.
 * @return int Código de error.
 * @retval 0 Éxito.
 * @retval 1 Error.
 */
int symtab_set(const char* key, symbol* sym);

/**
 * @brief Busca un símbolo en la Tabla de símbolos por su nombre.
 *
 * @param key nombre del símbolo a buscar.
 * @return symbol* Puntero al símbolo encontrado. NULL en caso contrario.
 */
symbol* symtab_get(const char* key);

/**
 * @brief Borra todas las variables y funciones definidas por el usuario en la
 * sesión. No se ven afectadas las librerías abiertas ni las funciones cargadas
 * dinámicamente.
 *
 * @return int Código de error.
 * @retval 0 Éxito.
 * @retval 1 Error.
 */
int symtab_reset(void);

/**
 * @brief Imprime el contenido completo de la Tabla de Símbolos.
 */
void symtab_print();

/**
 * @brief Imprime todas las variables, constantes y funciones  registradas en la
 * tabla de símbolos.
 */
void symtab_print_env(void);

/**
 * @brief Imprime todas las librerías abiertas.
 */
void symtab_print_libs(void);
