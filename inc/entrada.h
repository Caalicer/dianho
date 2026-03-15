#pragma once

/**
 * @file lexico.h
 * @brief Definición de las funciones públicas para el Análisis Léxico.
 *
 **/

/**
* @brief Inicializa el sistema de entrada.
* @param filename El nombre del archivo a analizar.
*/
void input_init(const char* filename);

/**
 * @brief Termina el analizador de entrada y libera los recursos utilizados.
 */
void input_terminate();

/**
 * @brief Obtiene el siguiente carácter.
 * @return El siguiente carácter del archivo o EOF si se ha llegado al final.
 */
char next_character();
