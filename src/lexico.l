%{
    // Importamos as librerías necesarias
    #include "lexico.h"
    #include "tabla_simbolos.h"
    #include "errores.h"
    #include "definiciones.h"
    int nesting_stack = 0;
%}

%x nesting
%option yylineno noyywrap noinput nounput

/* GENERALES*/
Character        (.|\n)
Letter           [a-zA-Z]
HexLetter        [a-fA-F]
EndOfLine        (\r|\n|\r\n)

Digit            [0-9]
NonCeroDigit     [1-9]
BinaryDigit      [01]
OctalDigit       [0-7]
HexDigit         {Digit}|{HexLetter}

EscapeSimple    \\[\'\"\\?abfnrtv0]
/*'*/
EscapeHex           \\x{HexDigit}{2}
EscapeOctal         \\{OctalDigit}{1,3}
EscapeUnicode4      \\u{HexDigit}{4}
EscapeUnicode8      \\U{HexDigit}{8}
/* Falta NamedCharacterEntity*/
EscapeSequence      ({EscapeSimple}|{EscapeHex}|{EscapeOctal}|{EscapeUnicode4}|{EscapeUnicode8})

/* IDENTIFICADORES */
IdentifierStart  _|{Letter}
IdentifierChar   {IdentifierStart}|0|{NonCeroDigit}
IdentifierChars  {IdentifierChar}+
IDENTIFIER       {IdentifierStart}{IdentifierChars}?

/* BINARIOS */
BinaryDigitUS          {BinaryDigit}|_
BinaryDigitsUS         {BinaryDigitUS}+
BinaryDigitsNoSingleUS {BinaryDigitsUS}?{BinaryDigit}{BinaryDigitsUS}?
BinPrefix              0b|0B
BinaryInteger          {BinPrefix}{BinaryDigitsNoSingleUS}

/* ENTEROS */
DecimalDigitUS       {Digit}|_
DecimalDigitsUS      {DecimalDigitUS}+
Underscores          _+
DecimalInteger       0{Underscores}?|{NonCeroDigit}{DecimalDigitsUS}?

IntegerSuffix        (L|u|U|Lu|LU|uL|UL)
BINARY_LITERAL       {BinaryInteger}{IntegerSuffix}?
DECIMAL_INTEGER      {DecimalInteger}{IntegerSuffix}?
/* Faltarían los hexadecimales */

/* FLOATS */
DecimalDigitsNoSingleUS   {DecimalDigitsUS}?{Digit}{DecimalDigitsUS}?
DecimalDigitsNoStartingUS {Digit}|{Digit}{DecimalDigitsUS}
DecimalExponentStart      [eE][+\-]?
DecimalExponent           {DecimalExponentStart}{DecimalDigitsNoSingleUS}
LeadingDecimal            {DECIMAL_INTEGER}|0{BinaryDigitsNoSingleUS}
EXPONENTIAL_FLOAT         ({LeadingDecimal}?\.{DecimalDigitsNoStartingUS}{DecimalExponent})|({LeadingDecimal}{DecimalExponent})
DECIMAL_FLOAT             ({LeadingDecimal}\.{DecimalDigitsNoStartingUS}?)|({LeadingDecimal}?\.{DecimalDigitsNoStartingUS})

/* STRINGS */
StringPostfix            [cwd]
DoubleQuotedCharacter    [^"\\\n]|{EscapeSequence} 
DoubleQuotedCharacters   ({DoubleQuotedCharacter})+
DoubleQuotedString       \"({DoubleQuotedCharacters})?\"({StringPostfix})?   
QUOTED_STRING            {DoubleQuotedString}
/*"*/

LineComment            "//"[^\n]*
BlockCommentContent    ([^*]|\*[^/])*
BlockComment           "/*"{BlockCommentContent}"*/"
Comment                {LineComment}|{BlockComment}

%%

{Comment}     /* Ignorar comentarios */
"/*"{BlockCommentContent} { emit_error(yylineno, UNFINISHED_BCOMMENT); }

"/+"	                  {BEGIN(nesting); nesting_stack++;}
<nesting>"+/"	          { nesting_stack--; if (nesting_stack == 0) BEGIN(INITIAL);}
<nesting>"/+"	          { nesting_stack++;}
<nesting>{Character}      {}
<nesting><<EOF>>          { emit_error(yylineno, UNFINISHED_NCOMMENT); yyterminate(); }


"..."   { return DDDOT; } /* Puntos */
".."    { return DDOT; }
"."     { return '.'; }
"++"    { return PP; }    /* Aritmética */
"+="    { return PE; }
"+"     { return '+'; }
"--"    { return MM; }
"-="    { return ME; }
"-"     { return '-'; }
"*="    { return SE; }
"*"     { return '*'; }
"/="    { return DE; }
"/"     { return '/'; }
"=="    { return EE; }    /* Comparación */
"<<="   { return LLE; }
"<<"    { return LL; }
"<="    { return LE; }
"<"     { return '<'; }
">>>="  { return GGGE; }
">>>"   { return GGG; }
">>="   { return GGE; }
">>"    { return GG; }
">="    { return GE; }
">"     { return '>'; }
"=>"    { return EG; }    /* Asignación */
"="     { return '='; }
"!"     { return '!'; }   /* Otros */
"~"     { return '~'; }
"@"     { return '@'; }
"?"     { return '?'; }
","     { return ','; }
";"     { return ';'; }
":"     { return ':'; }
"$"     { return '$'; }
"("     { return '('; }
")"     { return ')'; }
"["     { return '['; }
"]"     { return ']'; }
"{"     { return '{'; }
"}"     { return '}'; }

{IDENTIFIER}        { return ID; }

{EXPONENTIAL_FLOAT} { return EXPONENTIAL_LITERAL; }
{DECIMAL_FLOAT}     { return FLOAT_LITERAL; }
{DECIMAL_INTEGER}   { return INTEGER_LITERAL; }
{BINARY_LITERAL}    { return BINARY_LITERAL; }
{QUOTED_STRING}     { return STRING_LITERAL; }

[ \t\r\n]+      { }

.               { emit_error(yylineno, UNKNOWN_SYMBOL); } /* Error */

<<EOF>>                                             { yyterminate(); }

%%

void input_open(const char* nome){
    if((yyin = fopen(nome, "r")) == NULL) {
        emit_error(0, FILE_OPEN_ERROR);
        exit(1);
    }
}

void input_close(){
    fclose(yyin);
    yylex_destroy();
}

lexeme* next_token() {
    lexeme* token;
    if ((token = malloc(sizeof(lexeme))) == NULL) {
        emit_error(yylineno, MEMORY_ALLOCATION_ERROR);
        return NULL;
    }

    token->lexical_token = yylex();

    if (token->lexical_token == 0) { // EOF
        token->lexeme = NULL;
        return token;
    }

    token->lexeme = strndup(yytext, yyleng);

    if (token->lexical_token == ID) {
        return symtab_intern(token);
    }

    return token;
}