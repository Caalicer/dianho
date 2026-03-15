#pragma once

/**
 * @file lexico.h
 * @brief Definición de las funciones públicas para el Análisis Léxico.
 *
 **/

#include "definiciones.h"


/**
 * @brief Inicializa el analizador léxico con el archivo de entrada.
 *
 * @param filename El nombre del archivo a analizar.
 * @return 0 si la inicialización fue exitosa, -1 en caso de error.
 */
int init_lexical_analyzer(const char* filename);


/**
 * @brief Obtiene el siguiente token del análisis léxico.
 *
 * @return Un puntero a un lexema que representa el siguiente token, o NULL si no hay más tokens.
 */
lexeme* next_token();
