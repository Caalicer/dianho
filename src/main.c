#include <stdio.h>
#include <stdlib.h>

#include "dianho.h" // Nombre de nuestro Intérprete.

int main(int argc, char** argv) {
    setvbuf(stdout, NULL, _IONBF, 0);

    if (argc != 2) {
        fprintf(stderr, "Uso: %s <input_file>\n", argv[0]);
        return 1;
    }

    // Inicialización
    // printf("Preparando el sistema...\n");
    // symtab_init();

    // printf("\nEstado inicial de la tabla de símbolos:\n");
    // symtab_print();

    // Análisis

    printf("\n---- Inicio Intérprete ----\n");
    
    dianho(); // Iniciamos nuestro interprete.

    printf("\n---- Fin Intérprete ----\n");

    // printf("\nEstado final de la tabla de símbolos:\n");
    // symtab_print();

    // Finalizacion
    // symtab_terminate();

    return 0;
} 