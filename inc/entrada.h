#pragma once

/**
 * @file lexico.h
 * @brief Definición de las funciones públicas para el Análisis Léxico.
 *
 * > [!CAUTION] Sobre el uso de EOF con char
 * >
 * > EOF se define como un int de valor -1.
 * > Un char es de un byte, por lo que puede tomar valores de 0 a 255
 * > Si a un char le damos el valor de EOF (char a = EOF), el EOF al castearse
 * > se convertirá en 255. Por lo tanto, asumimos que en nuestro sistema de
 * > entrada jamás aparecerá el char 255.
 **/

#include "definiciones.h"
#include <stddef.h>

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
 * @brief Obtiene el siguiente carácter.
 * @return El siguiente carácter del archivo o EOF si se ha llegado al final.
 */
char input_getc();

/**
 * @brief Obtiene el siguiente carácter sin avanzar el puntero.
 * @return El siguiente carácter del archivo o EOF si se ha llegado al final.
 * @note [!CAUTION] Esta función puede forzar cargas de bloque, por lo que se
 *                  recomienda usarla con precaución.
 */
char input_peek();

/**
 * @brief Retrocede el puntero de lectura en una posición.
 */
void input_ungetc();

/**
 * @brief Obtiene los fragmentos del token actual.
 *
 * @return const fragments* Punteros a los fragmentos del token actual.
 * @note [!IMPORTANT] El contenido apuntado por los fragmentos es al propio
 *                    buffer de entrada
 */
const fragments* input_fragments();

/// unifica punteros
/**
 * @brief Acepta el lexema actual. Actualiza el inicio a la par que el final
 *        acutal, preparandose para el siguiente token.
 */
void input_advance();