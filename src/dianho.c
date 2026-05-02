#include "dianho.h"
#include <stdio.h>
#include "interprete.tab.h"


void dianho() {
    
    printf("\t DIANHO \n");
    
    yyparse();
}
