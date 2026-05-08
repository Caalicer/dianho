%{
    // Importamos as librerías necesarias
    #include <ctype.h>
    #include "lexico.h"
    #include "tabla_simbolos.h"
    #include "errores.h"
    #include "interprete.tab.h"
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
EscapeHex           \\x{HexDigit}{2}
EscapeOctal         \\{OctalDigit}{1,3}
EscapeUnicode4      \\u{HexDigit}{4}
EscapeUnicode8      \\U{HexDigit}{8}
/* Falta NamedCharacterEntity*/
EscapeSequence      ({EscapeSimple}|{EscapeHex}|{EscapeOctal}|{EscapeUnicode4}|{EscapeUnicode8})

/* OPERADORES INDIVIDUALES*/

SINGLE_OPERATOR   [+\-*/%=><!&|^,;()]


/* IDENTIFICADORES */
IdentifierStart  _|{Letter}
IdentifierChar   {IdentifierStart}|0|{NonCeroDigit}
IdentifierChars  {IdentifierChar}+
IDENTIFIER       {IdentifierStart}{IdentifierChars}?

/* BINARIOS */
BinaryDigitUS          {BinaryDigit}|_
BinaryDigitsUS         {BinaryDigitUS}+
BinaryDigitsNoSingleUS {BinaryDigitsUS}?{BinaryDigit}{BinaryDigitsUS}?

/* ENTEROS */
DecimalDigitUS       {Digit}|_
DecimalDigitsUS      {DecimalDigitUS}+
Underscores          _+
DecimalInteger       0{Underscores}?|{NonCeroDigit}{DecimalDigitsUS}?

IntegerSuffix        (L|u|U|Lu|LU|uL|UL)
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

/* BOOLEAN */

True    [Tt][Rr][Uu][Ee]
False   [Ff][Aa][Ll][Ss][Ee]
BOOLEAN {True}|{False}
/*"*/

LineComment            "//"[^\n]*
/* Limitamos comentarios a una linea */
/* BlockCommentContent    ([^*\n]|\ *[^/\n])* */
/* BlockComment           "/ *"{BlockCommentContent}"*/
Comment                {LineComment}

%%

{Comment}     /* Ignorar comentarios */



"**"    { return POW; }
"+="    { return PE; }
"-="    { return ME; }
"*="    { return SE; }
"/="    { return DE; }
">="    { return GE; }
"<="    { return LE; }
"=="    { return EE; }
"!="    { return NE; }
"&&"    { return AND; } 
"||"    { return OR; }
"<<"    { return LSHIFT; }
">>"    { return RSHIFT; }

{SINGLE_OPERATOR}   {return (int) *yytext;}

{EXPONENTIAL_FLOAT} { yylval.fval = atof(yytext); return EXPONENTIAL_LITERAL; }
{DECIMAL_FLOAT}     { yylval.fval = atof(yytext); return FLOAT_LITERAL; }
{DECIMAL_INTEGER}   { yylval.ival = atoll(yytext); return INTEGER_LITERAL; }
{BOOLEAN}           { yylval.bval = (tolower(yytext[0]) == 't'); return BOOLEAN_LITERAL; }
{QUOTED_STRING}     { yylval.sval = strdup(yytext); return STRING_LITERAL; }

{IDENTIFIER}        { yylval.sval = strdup(yytext); return ID; }
\n                  { return NEWLINE; }

[ \t\r]+      { }

.               { emit_error(yylineno, UNKNOWN_SYMBOL); } /* Error */

<<EOF>>                                             { yyterminate(); }

%%