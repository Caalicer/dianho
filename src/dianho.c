#include "dianho.h"
#include <stdio.h>
#include "interprete.tab.h"
#include <unistd.h>

void print_logo();


void dianho() {

    print_logo();

    yyparse();
}


void print_logo(){

    printf("    ══════════════════════════════════════════════════\n");
    printf("    ▓█████▄  ██▓ ▄▄▄       ███▄    █  ██░ ██  ▒█████  \n");
    printf("    ▒██▀ ██▌▓██▒▒████▄     ██ ▀█   █ ▓██░ ██▒▒██▒  ██▒\n");
    printf("    ░██   █▌▒██▒▒██  ▀█▄  ▓██  ▀█ ██▒▒██▀▀██░▒██░  ██▒\n");
    printf("    ░▓█▄   ▌░██░░██▄▄▄▄██ ▓██▒  ▐▌██▒░▓█ ░██ ▒██   ██░\n");
    printf("    ░▒████▓ ░██░ ▓█   ▓██▒▒██░   ▓██░░▓█▒░██▓░ ████▓▒░\n");
    printf("     ▒▒▓  ▒ ░▓   ▒▒   ▓▒█░░ ▒░   ▒ ▒  ▒ ░░▒░▒░ ▒░▒░▒░ \n");
    printf("     ░ ▒  ▒  ▒ ░  ▒   ▒▒ ░░ ░░   ░ ▒░ ▒ ░▒░ ░  ░ ▒ ▒░ \n");
    printf("     ░ ░  ░  ▒ ░  ░   ▒      ░   ░ ░  ░  ░░ ░░ ░ ░ ▒  \n");
    printf("       ░     ░        ░  ░         ░  ░  ░  ░    ░ ░  \n");
    printf("     ░                                                \n");
    printf("    ══════════════════════════════════════════════════\n");
}

void clear(){
    printf("\033[H\033[J");
}
