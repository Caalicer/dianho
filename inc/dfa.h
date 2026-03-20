#pragma once

/**
 * @brief Defininición de un Automata Finito Determinista (DFA).
 *
 * > [!NOTE] Estado inicial
 * > Se asume que el estado inicial siempre es el estado 0.
 *
 * > [!NOTE] Bitmap de aceptación
 * > Así la comprobación de aceptación es O(1).
 *
 * Si definimos los estados como enum estados { s_1, s_2, s_3, s_4, s_5 }
 *
 * El bitmap de aceptación para los estados 2 y 5 sería:
 * accepting_bitmap = (1ULL << s_2) | (1ULL << s_5);
 *
 * > [!WARNING] Número de estados
 * > Limitado a 64 estados por el uso del bitmap
 *
 * Se hace una clara ditinción entre:
 *
 * - *alfabeto* del dfa: elementos que consume el DFA para transicionar.
 * - símbolos de entrada*: valores de entrada reales.
 *
 * Esto permite agrupar diferente símbolos de entrada bajo un mismo elemento del
 * alfabeto del DFA.  Véase el ejemplo:
 *
 * Alfabeto del DFA: enum alfabeto{ NUMEROS, LETRAS }
 *
 * Símbolos de entrada: {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a',
 *                       'b', 'c', ..., 'z'}
 *
 * En este caso podriamos agrupar:
 * NUMEROS: {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'}
 * LETRAS: {'a', 'b', 'c', ..., 'z'}

 * Para lograr esto, se usa la `mapping_table`. Se trata de un array de enteros
 del tamaño de los símbolos de entrada, que establece esta relación.
 *
 * > [!IMPORTANT] mapping table
 * > los símbolos se deben poder a castear a enteros para poder
 * > usarlos como índice de la tabla. Por eso symbol se define como un char,
 * > pero podría ser también un *enum*
 *
 * De esta forma la tabla se construye:
 * mapping_table['0'] = NUMEROS; // numeros
 * mapping_table['1'] = NUMEROS; // numeros
 * ...
 * mapping_table['a'] = LETRAS; // letras
 * mapping_table['b'] = LETRAS; // letras
 * ...
 *
 */

#include <stddef.h>
#include <stdint.h>

/// Modificable solo por algo castebla a entero: char, enum...
typedef char symbol;

/*****************************
 * Definición no modificable *
 *****************************/

typedef struct DFA DFA; // Ocultamos la estructura interna del DFA
typedef int state;      // Por legibilidad.

/**
 * @enum dfa_status
 * @brief Estado del DFA.
 * Indica si la transición se realizó correctamente y si
 * el estado actual es de aceptación.
 */
typedef enum {
    ACCEPTING,     /**< Estado de aceptación */
    REJECTING,     /**< Estado válido pero no de aceptación */
    NO_TRANSITION, /**< No existe transición definida para la entrada */
    NO_MAPPING,    /**< Símbolo de entrada no mapeado al alfabeto del DFA */
    ERROR          /**< Error en el último procesamiento. No se transicionó */
} dfa_status;

/**
 * @brief Crea e inicializa un nuevo DFA con los parámetros dados.
 *
 * @param n_states Número de estados.
 * @param n_alphabet Número de elementos en el alfabeto.
 * @param n_symbols Número de símbolos de entrada.
 * @param accepting_bitmap Bitmap de estados de aceptación.
 * @param transition_table Tabla de transición del DFA.
 * @param mapping_table Tabla de mapeo de símbolos de entrada al alfabeto.
 *
 * @return Puntero al DFA inicializado.
 */
DFA* dfa_init(size_t n_states, size_t n_alphabet, size_t n_symbols,
              uint64_t accepting_bitmap, const char* mapping_table,
              const short int transition_table[n_states][n_alphabet]);

/**
 * @brief Destruye el DFA, liberando cualquier recurso asociado.
 *
 * @param dfa puntero al DFA a destruir.
 */
void dfa_destroy(DFA* dfa);

/**
 * @brief Maneja una entrada para el DFA, transicionando entre estados.
 *
 * @param dfa Puntero al DFA que consumirá la entrada.
 * @param input Símbolo de entrada a procesar.
 * @return El status del DFA después de procesar la entrada.
 * @retval ACCEPTING si se encuentra en aceptación
 * @retval REJECTING si no se encuentra en aceptación
 * @retval NO_TRANSITION si no existe transición definida para la entrada
 * @retval NO_MAPPING símbolo de entrada no mapeado al alfabeto del DFA
 * @retval ERROR símbolo no válido
 */
dfa_status dfa_step(DFA* dfa, symbol input);

/**
 * @brief Obtiene el estado actual del DFA.
 *
 * @param dfa Puntero al DFA.
 * @return El estado actual del DFA. -1 si el DFA es NULL.
 */
state dfa_current_state(DFA* dfa);

/**
 * @brief Comprueba si el estado actual del DFA es un estado de aceptación.
 * 
 * @param dfa Puento al dfa
 * @return dfa_status Estado actual del DFA.
 * @retval -1 El DFA es NULL
 * @retval ERROR si el simbolo es desconocido
 * @retval NO_MAPPING si el simbolo es conocido pero esta mapeado
 * @retval NO_TRANSITION si el simbolo es conocido pero no hay transición
 * @retval ACCEPTING si el estado actual es de aceptación
 * @retval REJECTING si el estado actual no es de aceptación
 */
dfa_status dfa_is_accepting(DFA* dfa);

/**
 * @brief Reinicia el DFA a su estado inicial (estado 0).
 */
void dfa_reset(DFA* dfa);