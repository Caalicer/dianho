#pragma once

/**
 * @file lexico.h
 * @brief Definición de las funciones públicas para el Análisis Léxico.
 *
 **/

#include "definiciones.h"


/**
 * @brief Inicializa el analizador léxico: automatas y estructura interna.
 */
void lexer_init();

/**
 * @brief Finaliza el analizador léxico y libera los recursos utilizados.
 */
void lexer_terminate();

/**
 * @brief Termina el analizador léxico y libera los recursos utilizados.
 */
void lexer_terminate();

/**
 * @brief Obtiene el siguiente token del análisis léxico.
 *
 * @return Lexema que representa el siguiente token
 * @retval Lexema con comp_lexico == EOF si llega al final del archivo
 */
lexeme* next_token();
