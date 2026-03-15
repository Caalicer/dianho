#include "dfa.h"
#include <stdint.h>
#include <stdlib.h>

/**
 * @struct Autómata Finito Determinista (DFA)
 * @brief  Estructuración Autómata Finito Determinista (DFA)
 *
 */
struct DFA {
    size_t n_states;             /**< Número de estados */
    size_t n_alphabet;           /**< Número de elementos en el alfabeto */
    size_t n_symbols;            /**< Número de símbolos de entrada */
    state current_state;         /**< Estado actual */
    uint32_t accepting_bitmap;   /**< Bitmap estados aceptación */
    const int* transition_table; /**< Tabla de transición de estados */
    const int* mapping_table;    /**< Tabla de mapeo al alfabeto */
};

DFA* dfa_init(size_t n_states, size_t n_alphabet, size_t n_symbols,
              uint32_t accepting_bitmap, const int* mapping_table,
              const int transition_table[n_states][n_alphabet]) {

    DFA* dfa = malloc(sizeof(DFA));
    if (!dfa) {
        return NULL;
    }

    dfa->n_states = n_states;
    dfa->n_alphabet = n_alphabet;
    dfa->n_symbols = n_symbols;
    dfa->current_state = 0; // Estado inicial siempre 0
    dfa->transition_table = (const int*)transition_table; // Flattened
    dfa->accepting_bitmap = accepting_bitmap;
    dfa->mapping_table = mapping_table;

    return dfa;
}

void dfa_destroy(DFA* dfa) {
    if (dfa) {
        free(dfa);
    }
}

dfa_status dfa_step(DFA* dfa, symbol input) {

    if (!dfa || input < 0 || (size_t)input >= dfa->n_symbols) {
        return ERROR; // Entrada no válida
    }

    int row = (int)dfa->current_state;
    int col = dfa->mapping_table[(int)input];

    if (col < 0) {
        return ERROR; // Símbolo no mapeado
    }

    dfa->current_state = dfa->transition_table[row * dfa->n_alphabet + col];

    return (dfa->accepting_bitmap & (1ULL << dfa->current_state)) ? ACCEPTING
                                                                  : REJECTING;
}

state dfa_current_state(DFA* dfa) { return dfa ? dfa->current_state : -1; }