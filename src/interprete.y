%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definiciones.h"

int yylex(void);
void yyerror(const char *s);
%}

%union {
    char *str;
}

/* Tokens con valor string */
%token <str> ID
%token <str> EXPONENTIAL_LITERAL FLOAT_LITERAL INTEGER_LITERAL
%token <str> BOOLEAN_LITERAL STRING_LITERAL

/* Operadores compuestos con valor string */
%token <str> PE ME SE DE GE LE EE NE OR AND LSHIFT RSHIFT POW

/* Tokens sin valor semántico */
%token NEWLINE

%type <str> exp token single_op
%destructor { free($$); } <str>

%%

input:
    %empty { printf("Welcome to DIANHO! Type your commands below:\n> "); }
|   input line
;

line:
    NEWLINE      { printf("> "); }   
|   exp NEWLINE  { printf("echo: %s\n> ", $1); free($1); }
;

exp:
    token
    {
        $$ = $1;
    }
|   exp token
    {
        $$ = malloc(strlen($1) + strlen($2) + 2);
        sprintf($$, "%s %s", $1, $2);
        free($1);
        free($2);
    }
;

token:
    ID                  { $$ = $1; }
|   EXPONENTIAL_LITERAL { $$ = $1; }
|   FLOAT_LITERAL       { $$ = $1; }
|   INTEGER_LITERAL     { $$ = $1; }
|   BOOLEAN_LITERAL     { $$ = $1; }
|   STRING_LITERAL      { $$ = $1; }
|   PE                  { $$ = $1; }
|   ME                  { $$ = $1; }
|   SE                  { $$ = $1; }
|   DE                  { $$ = $1; }
|   GE                  { $$ = $1; }
|   LE                  { $$ = $1; }
|   EE                  { $$ = $1; }
|   NE                  { $$ = $1; }
|   OR                  { $$ = $1; }
|   AND                 { $$ = $1; }
|   LSHIFT              { $$ = $1; }
|   RSHIFT              { $$ = $1; }
|   POW                 { $$ = $1; }
|   single_op
;

single_op:
    '+'  { $$ = strdup("+"); }
|   '-'  { $$ = strdup("-"); }
|   '*'  { $$ = strdup("*"); }
|   '/'  { $$ = strdup("/"); }
|   '%'  { $$ = strdup("%"); }
|   '='  { $$ = strdup("="); }
|   '>'  { $$ = strdup(">"); }
|   '<'  { $$ = strdup("<"); }
|   '!'  { $$ = strdup("!"); }
|   '&'  { $$ = strdup("&"); }
|   '|'  { $$ = strdup("|"); }
|   '^'  { $$ = strdup("^"); }
|   ','  { $$ = strdup(","); }
|   ';'  { $$ = strdup(";"); }
|   '('  { $$ = strdup("("); }
|   ')'  { $$ = strdup(")"); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}