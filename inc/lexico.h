#pragma once

/**
 * @file lexico.h
 * @brief Definición de las funciones públicas para el Análisis Léxico.
 *
 **/

#include "definiciones.h"

/**
 * @brief Inicializa el sistema de entrada.
 * @param filename El nombre del archivo a analizar.
 */
void input_open(const char* filename);

/**
 * @brief Termina el analizador de entrada y libera los recursos utilizados.
 */
void input_close();

/**
 * @brief Obtiene el siguiente token del análisis léxico.
 *
 * @return Lexema que representa el siguiente token
 * @retval Lexema con comp_lexico == EOF si llega al final del archivo
 */
lexeme* next_token();
