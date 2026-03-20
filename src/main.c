#include <stdio.h>
#include <stdlib.h>

#include "entrada.h"
#include "tabla_simbolos.h"
#include "sintactico.h"
#include "lexico.h"

int main(int argc, char** argv) {
    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Inicialización
    printf("Preparando el sistema...\n");
    input_open(argv[1]);
    symtab_init();
    lexer_init();

    printf("\nEstado inicial de la tabla de símbolos:\n");
    symtab_print();

    // Análisis

    printf("\n---- Inicio Análisis ----\n");
    
    parse();

    printf("\n---- Fin Análisis ----\n");

    printf("\nEstado final de la tabla de símbolos:\n");
    symtab_print();

    // Finalizacion
    symtab_terminate();
    input_close();

    return 0;
} 