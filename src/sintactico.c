#include <stdbool.h>
#include <stdio.h>

#include "definiciones.h"
#include "lexico.h"

void parse() {

    lexeme* token;
    bool continue_parsing = true;

    while (continue_parsing) {

        token = next_token();

        if (token->lexical_token == EOF)
            continue_parsing = false;
        else
            print_lexeme(*token);

    }
}