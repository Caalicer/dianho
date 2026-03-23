#include <stdbool.h>
#include <stdio.h>

#include "definiciones.h"
#include "lexico.h"
#include "tabla_simbolos.h"

void parse() {

    lexeme* token;
    bool continue_parsing = true;

    while (continue_parsing) {

        token = next_token();

        if (token->lexical_token == 0) { // EOF
            continue_parsing = false;
            free (token->lexeme);
            free(token);
        } else {
            print_lexeme(*token);
            if (symtab_lookup(token) == NULL) {
                free(token->lexeme);
                free(token);
            }
        }
    }
}