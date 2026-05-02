#pragma once

/**
 * @file lexico.h
 * @brief Definición de las funciones públicas para el Análisis Léxico.
 *
 **/

#include "definiciones.h"


/**
 * @brief Obtiene el siguiente token del análisis léxico.
 *
 * @return Lexema que representa el siguiente token
 * @retval Lexema con comp_lexico == EOF si llega al final del archivo
 */
lexeme* next_token();
