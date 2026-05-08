%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "definiciones.h"

int yylex(void);
void yyerror(const char *s);
%}


%union {
    long long ival;
    double    fval;
    int       bval;
    char     *sval;
}

%token <ival> INTEGER_LITERAL
%token <fval> FLOAT_LITERAL EXPONENTIAL_LITERAL
%token <bval> BOOLEAN_LITERAL
%token <sval> STRING_LITERAL ID


%type <fval> exp

/* Operadores compuestos con valor string */
%token <str> PE ME SE DE GE LE EE NE OR AND LSHIFT RSHIFT POW

/* Tokens sin valor semántico */
%token NEWLINE



%left OR
%left AND
%left '|'
%left '^'
%left '&'
%left EE NE
%left '<' '>' LE GE
%left LSHIFT RSHIFT
%left '+' '-'
%left '*' '/' '%'
%right POW
%right UMINUS '!'


%%

input:
    %empty { printf("> "); }
|   input line
;

line:
    NEWLINE
|   exp NEWLINE  { printf("= %g\n> ", $1); }
;

exp:
    INTEGER_LITERAL        { $$ = (double)$1;          }
|   FLOAT_LITERAL          { $$ = $1;                  }
|   EXPONENTIAL_LITERAL    { $$ = $1;                  }
|   '(' exp ')'            { $$ = $2;                  }
|   '-' exp %prec UMINUS   { $$ = -$2;                 }
|   exp '+' exp            { $$ = $1 + $3;             }
|   exp '-' exp            { $$ = $1 - $3;             }
|   exp '*' exp            { $$ = $1 * $3;             }
|   exp '/' exp            { $$ = $1 / $3;             }
|   exp '%' exp            { $$ = (long long)$1 % (long long)$3; }
|   exp POW exp            { $$ = pow($1, $3);         }
|   exp '>' exp            { $$ = $1 > $3;             }
|   exp '<' exp            { $$ = $1 < $3;             }
|   exp GE  exp            { $$ = $1 >= $3;            }
|   exp LE  exp            { $$ = $1 <= $3;            }
|   exp EE  exp            { $$ = $1 == $3;            }
|   exp NE  exp            { $$ = $1 != $3;            }
|   exp AND exp            { $$ = $1 && $3;            }
|   exp OR  exp            { $$ = $1 || $3;            }
|   exp '&' exp            { $$ = (long long)$1 & (long long)$3; }
|   exp '|' exp            { $$ = (long long)$1 | (long long)$3; }
|   exp '^' exp            { $$ = (long long)$1 ^ (long long)$3; }
|   exp LSHIFT exp         { $$ = (long long)$1 << (long long)$3; }
|   exp RSHIFT exp         { $$ = (long long)$1 >> (long long)$3; }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    yyparse(); // Reiniciar el parser para continuar leyendo
}