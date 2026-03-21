#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "automatas.h"
#include "definiciones.h"
#include "dfa.h"
#include "entrada.h"
#include "errores.h"
#include "lexico.h"
#include "tabla_simbolos.h"

/**
 * @macro HANDLER_SPEC
 * @brief Inicializa un literal de `handler_spec` con DFA, callbacks y enum.
 *
 * @param dfa_ptr Puntero al DFA.
 * @param accept  Callback on_accept (puede ser NULL).
 * @param fail    Callback on_fail (puede ser NULL).
 * @param eof     Callback on_eof (puede ser NULL).
 * @param enum_name Enum del handler.
 */
#define HANDLER_SPEC(dfa_ptr, start, accept, no_transition)                    \
    (handler_spec) {                                                           \
        .automata = dfa_ptr, .ops = { start, accept, no_transition }           \
    }
/**
 * @struct automata_handler_ctx Contexto de variables *internas* necesarias para
 *         el manejo de los automatas.
 * @brief Permite compartir el estado del handler principal con las funciones de
 *        cada variable.
 */
typedef struct {
    size_t token_len; ///< Longitud del token actual
    dfa_status status;
    bool is_recursive;     ///< El token es un comentario recursivo
    bool continue_reading; ///< Controlar la lectura de caracteres en el handler
    int nesting_stack;     ///< Pila de anidamiento en comentarios
    char c;
    state last_state;
} handler_ctx;

/**
 * @struct automata_handler_ops Callback para un handler genérico.
 * @brief Permite ajustar el comportamiento del handler genérico en función del
 *        contexto (tipo de autómata que gestiona)
 */
typedef struct {
    void (*on_start)(handler_ctx* ctx);
    void (*on_accept)(handler_ctx* ctx);
    void (*on_no_transition)(handler_ctx* ctx);
} handler_ops;

/**
 * @struct automata_handler_spec Especificaciones para la llamda al handler.
 * @brief Asocia las variables de entrada necesarias para llamar al handler.
 *        Nos permitirá crear un mapper para cada autómata
 */
typedef struct {
    DFA* automata;   ///< Automata a gestionar
    handler_ops ops; ///< Comportamiento específico del automata
} handler_spec;

/**
 * @brief Clasifica los char en tipos para facilitar la identificación del tipo
 * de token y el automata que se debe ejecutar.
 */
implemented_dfa _char_classify(char c);

DFA* alphanumeric; ///< DFA para ID y Keywords
DFA* numeric;      ///< DFA para numeros y DOT DDOT DDDOT
DFA* comments;     ///< DFA para comentarios
DFA* atomics;      ///< DFA para operadores y puntuación
DFA* strings;      ///< DFA para double quoted strings

/**
 * @brief Handler genérico para alfanuméricos, numéricos, comentarios,
 *        operadores y strings
 */
const fragments* _automata_handler(DFA* automata, handler_ops* ops);

void _h_void(handler_ctx* ctx);
void _h_accept(handler_ctx* ctx);
void _h_comments(handler_ctx* ctx);
void _h_string(handler_ctx* ctx);
void _h_num_error(handler_ctx* ctx);
void _h_atomic_error(handler_ctx* ctx);

/*******************************
 *   VARIABLES GLOBALES LÉXICO *
 *******************************/

size_t line_number; ///< Número de línea actual, se incrementa al leer '\n'
handler_spec
    specs[DFA_COUNT]; ///< Mapeo de variables de entrada para cada autómata
short int*
    state_mapper[DFA_COUNT]; ///< Tabla con los mapas de los estados de cada
                             ///< automata al respectivo TOKEN y ERROR

/****************************
 *    FUNCIONES PÚBLICAS    *
 ****************************/
void lexer_init() {

    line_number = 1;

    alphanum_mapper();
    numeric_mapper();
    comments_mapper();
    atomics_mapper();
    strings_mapper();

    alphanumeric =
        dfa_init(anq_COUNT_STATES, ana_COUNT_SYMBOLS, 256,
                 alphanum_accpeting_bitmap, alphanumeric_map, alphanum_trans);

    numeric = dfa_init(nq_COUNT_STATES, na_COUNT_SYMBOLS, 256,
                       numeric_accpeting_bitmap, numeric_map, numeric_trans);

    comments =
        dfa_init(cq_COUNT_STATES, ca_COUNT_SYMBOLS, 256,
                 comments_accpeting_bitmap, comments_map, comments_trans);

    atomics = dfa_init(aq_COUNT_STATES, aa_COUNT_SYMBOLS, 256,
                       atomics_accpeting_bitmap, atomics_map, atomics_trans);

    strings = dfa_init(sq_COUNT_STATES, sa_COUNT_SYMBOLS, 256,
                       strings_accpeting_bitmap, strings_map, strings_trans);

    specs[ALPHANUMERIC] = HANDLER_SPEC(alphanumeric, _h_void, _h_void, _h_void);
    specs[NUMERIC] = HANDLER_SPEC(numeric, _h_void, _h_void, _h_num_error);
    specs[COMMENTS] = HANDLER_SPEC(comments, _h_comments, _h_accept, _h_void);
    specs[ATOMICS] = HANDLER_SPEC(atomics, _h_void, _h_void, _h_atomic_error);
    specs[STRINGS] = HANDLER_SPEC(strings, _h_string, _h_accept, _h_void);

    state_mapper[ALPHANUMERIC] = alphanumeric_token;
    state_mapper[NUMERIC] = numeric_token;
    state_mapper[COMMENTS] = comments_token;
    state_mapper[ATOMICS] = atomics_token;
    state_mapper[STRINGS] = strings_token;
}

void lexer_terminate() {
    dfa_destroy(alphanumeric);
    dfa_destroy(numeric);
    dfa_destroy(comments);
    dfa_destroy(atomics);
    dfa_destroy(strings);
}

lexeme* next_token() {
    lexeme* token;
    const fragments* token_fragments = NULL;
    bool token_found = false;
    implemented_dfa type;

    if ((token = malloc(sizeof(lexeme))) == NULL) {
        emit_error(line_number, MEMORY_ALLOCATION_ERROR);
        return NULL;
    }

    while (!token_found) {

        type = _char_classify(input_peek());

        switch (type) {
            case D_EOF:
                token->lexical_token = EOF;
                token->lexeme = NULL;
                return token;

            case WHITESPACE: {
                char c;
                while (isspace(c = input_getc())) {
                    if (c == '\n')
                        line_number++;
                }
                input_ungetc(); // El último consumido no es espacio.
                break;
            }
            case UNKNOWN:
                emit_error(line_number, UNKNOWN_SYMBOL);
                input_getc(); // Consumimos el caracter desconocido
                break;

            default:
                input_advance(); // Inicio de token, unificamos centinelas
                token_fragments =
                    _automata_handler(specs[type].automata, &specs[type].ops);

                // Condicional tricky.
                if (type != COMMENTS) // Los comentarios no emiten token
                    token_found = true;
                // Pero dentro del dfa de comentarios detectamos dos atómicos
                else if (state_mapper[type][dfa_current_state(
                             specs[type].automata)] == '/' ||
                         state_mapper[type][dfa_current_state(
                             specs[type].automata)] == DE)
                    // Dentro de los comentarios, es '/' o '/='
                    token_found = true;
        }
    }

    if (dfa_is_accepting(specs[type].automata) == REJECTING) {
        emit_error(
            line_number,
            (error_code)
                state_mapper[type][dfa_current_state(specs[type].automata)]);
        token->lexical_token = TOKEN_ERROR; // Sabemos que el token es erroneo.
    } else {
        token->lexical_token =
            state_mapper[type][dfa_current_state(specs[type].automata)];
        if (type == ALPHANUMERIC) {
            lexeme* interned = symtab_fragments_lookup(token_fragments);
            if (interned != NULL) {
                free(token);
                return interned;
            }
            token->lexeme = fragments_to_string(token_fragments);
            symtab_intern(token);
        }
    }

    if (type != ALPHANUMERIC) {
        token->lexeme = fragments_to_string(token_fragments);
    }

    return token;
}

/****************************
 *    FUNCIONES INTERNAS    *
 ****************************/
implemented_dfa _char_classify(char c) {
    if (c == EOF)
        return D_EOF;
    if (isspace(c) || c == '\n')
        return WHITESPACE;
    if (isalpha(c) || c == '_')
        return ALPHANUMERIC;
    if (isdigit(c) || c == '.')
        return NUMERIC;
    if (c == '"')
        return STRINGS;
    if (c == '/')
        return COMMENTS;
    if (c == '+' || c == '-' || c == '*' || c == '<' || c == '>' || c == '=' ||
        c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' ||
        c == ';' || c == ',')
        return ATOMICS;
    return UNKNOWN;
}

const fragments* _automata_handler(DFA* automata, handler_ops* ops) {

    handler_ctx ctx = {
        .token_len = 0, .is_recursive = false, .continue_reading = true};

    dfa_reset(automata);
    while (ctx.continue_reading) { // Avanzamos en el automata hasta aceptar

        ctx.c = input_getc();
        ctx.status = dfa_step(automata, ctx.c);
        ctx.last_state = dfa_current_state(automata);

        if (ctx.c == '\n')
            line_number++;

        ops->on_start(&ctx); // Gestionamos: COMMENTS -> pila anidamiento y EOF
                             //              STRINGS  -> EOF
        switch (ctx.status) {
            case ACCEPTING:
                ctx.token_len++;
                ops->on_accept(&ctx); // String y comments al aceptar paran
                break;

            case REJECTING:
                ctx.token_len++; // El caracter pertenece al token
                break;

            case NO_TRANSITION:
                ops->on_no_transition(&ctx); // Para numeric: emite errores
                /* fallthrough */
            case NO_MAPPING:
                if (ctx.c == '\n')
                    line_number--;
                input_ungetc(); // Leimos uno demas
                ctx.continue_reading = false;
                break;

            case ERROR:
                emit_error(line_number, UNKNOWN_SYMBOL);
                ctx.continue_reading = false;
                break;

            default:
                emit_error(line_number, NOT_INITIALIZED);
                ctx.continue_reading = false;
                break;
        }

        /// Cortamos el token y se empieza a formar uno nuevo desde el siguiente
        /// caracter. Esto puede ocurrir, por ejemplo, en medio de un comentario
        /// anidado o string, probocando que el resto del archivo se tokenize de
        /// forma extraña.
        if (ctx.token_len >= MAX_TOKEN_LENGTH) {
            emit_error(line_number, LEXEME_TOO_LONG);
            if (ctx.c == '\n')
                line_number--;
            input_ungetc(); // TODO revisar si hay que devolverlo
            ctx.continue_reading = false;
            break;
        }
    }
    // printf("devuelvo fragmentos");
    return input_fragments();
}

void _h_void(handler_ctx* ctx) {
    (void)ctx;
    return;
}

void _h_accept(handler_ctx* ctx) {
    ctx->continue_reading = false;

    if (ctx->last_state == cq_D) {
        input_ungetc();   // El atómico '/' se acepta detectando uno demás.
        ctx->token_len--; // No contamos el extra
    }
}

void _h_comments(handler_ctx* ctx) {
    if (ctx->last_state == cq_recursive && !ctx->is_recursive) {
        ctx->is_recursive = true;
        ctx->nesting_stack = 1; // Primer nivel de anidamiento
    }
    if (ctx->is_recursive) { // Gestión pila anidamiento
        if (ctx->last_state == cq_push)
            ctx->nesting_stack++;
        if (ctx->last_state == cq_pop)
            ctx->nesting_stack--;
        if (ctx->nesting_stack == 0) // Anidamientos emparejados
            ctx->continue_reading = false;
    }
    if (ctx->c == EOF) { // LLegamos al final del archivo sin cerrar
        if (ctx->last_state == cq_line || ctx->last_state == cq_selector) {
            // El caso de cq_selector es tricky. En teoría no es posible que
            // termine un archivo correcto ahí, pero tendríamos que detectarlo.
            // Es un edge case raro y no merece la pena considerarlo. Supondría
            // tener que hacer un step mas en el dfa con un caracter basura al
            // azar como ' ' para que el estado final fuese el correcto y
            // pudiese detectar bien el token next_token con los mappers, y no
            // generar un ERROR y un lexema 999.
            // Actualmente si el fichero fuente termina en / emite un DFA error
            ctx->status = dfa_step(comments, '\n'); // Forzamos transición

        } else {
            emit_error(line_number, comments_token[ctx->last_state]);
        }
        input_ungetc();
        ctx->token_len++; // Porque despues en REJECTING lo cuenta
        ctx->continue_reading = false;
    }
}

void _h_string(handler_ctx* ctx) {
    if (ctx->c == EOF) { // LLegamos al final del archivo sin cerrar
        emit_error(line_number, UNFINISHED_STRING);
        input_ungetc();
        ctx->token_len++; // Porque despues en REJECTING lo cuenta
        ctx->continue_reading = false;
    }
}

void _h_num_error(handler_ctx* ctx) {
#ifdef WARNING
    emit_warning(line_number, MALFORMED_NUMBER);
#endif
    ctx->continue_reading = false;
}

void _h_atomic_error(handler_ctx* ctx) {
#ifdef WARNING
    emit_warning(line_number, MALFORMED_ATOMIC);
#endif
    ctx->continue_reading = false;
}