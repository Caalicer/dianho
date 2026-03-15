
#include "avl.h"
#include "definiciones.h"
#include "tabla_simbolos.h"
#include <stdio.h>
#include <string.h>
#include "sintactico.h"
#include "dfa.h"


// static void _print_element(element data) {
//     printf("%d ", data.lexical_token);
// }

typedef enum {
    s_inicial,
    s_1,
    s_2,
    s_aceptacion,
    s_count
} estados;

typedef enum {
    letra_a,
    letra_b,
    a_count
} alfabeto;

static const int tabla_transiciones[s_count][a_count] ={
                      // a    b
    [s_inicial]    = {s_1, s_inicial},
    [s_1]          = {s_1, s_2},
    [s_2]          = {s_1, s_aceptacion},
    [s_aceptacion] = {s_aceptacion, s_aceptacion}
};

uint64_t accepting_bitmap = (1ULL << s_aceptacion);

int map [256];


int main(int argc, char** argv) {

    // memset(map, -1, sizeof(map));
    // map ['a'] = letra_a;
    // map ['b'] = letra_b;

    // DFA *dfa = dfa_init(s_count, a_count, 255, accepting_bitmap, map, tabla_transiciones);

    // char input_char;

    // while(1) {
    //     printf("\nIntroduce letra:");
    //     scanf(" %c", &input_char); // Leer un carácter de entrada

    //     switch (dfa_step(dfa, input_char)) {
    //         case ACCEPTING:
    //             printf("Estado actual: Aceptación\n");
    //             break;
    //         case REJECTING:
    //             printf("Estado actual: Rechazo\n");
    //             break;
    //         case ERROR:
    //             printf("Entrada no válida. Intente de nuevo.\n");
    //             break;
    //     }

    //     printf("Estado actual del DFA: %d\n", dfa_current_state(dfa));

    // }
    // if (argc != 2) {
    //     fprintf(stderr, "Uso: %s <input_file>\n", argv[0]);
    //     return 1;
    // }

    symtab_init();

    symtab_print();

    symtab_terminate();

    // sintactic_analysis(argv[1]);

    // tabla_simbolos_print();

    // tabla_simobolos_terminate();

    return 0;
}