#include "tabla_simbolos.h"
#include <stdlib.h>
#include "avl.h"


AVLTree* tabla_simbolos;

void tabla_simbolos_init() { tabla_simbolos = avl_create(); }

void tabla_simobolos_terminate() { avl_destroy(tabla_simbolos); }

