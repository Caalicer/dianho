#include <asm-generic/errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definiciones.h"
#include "entrada.h"
#include "errores.h"

#define BUFFER_SIZE MAX_TOKEN_LENGTH
#define TOTAL_SIZE (BUFFER_SIZE * 2 + 2) // 2 mitades + 2 centinelas

/*******************************
 *      ESTADO INTERNO         *
 *******************************/

typedef enum { A, B, COUNT } half;

typedef struct half_info {
    size_t length; // bytes leídos
    bool eof;      // true si llegamos al final del archivo
    bool used;     // Para evitar machacar una mitad si solo se hizo peek
} half_info;

static FILE* src = NULL;

static char buffer[2][BUFFER_SIZE + 1]; // +1 para centinela
static half_info half_status[2] = {{0, false, true}, {0, false, true}};

static char* lexeme_begin = NULL;
static char* forward = NULL;

static fragments frags; ///< Fragmentos del token actual

/*******************************
 *    FUNCIONES INTERNAS       *
 *******************************/

/**
 * @brief Carga un bloque del fichero en la mitad indicada.
 *        Coloca centinela EOF al final de la mitad.
 */
static void _load_half(half h) {
    if (half_status[h].used) {
        half_status[h].length = fread(buffer[h], 1, BUFFER_SIZE, src);
        half_status[h].eof =
            (half_status[h].length < BUFFER_SIZE); // EOF fichero
        buffer[h][half_status[h].length] = EOF;    // Centinela
    }
}

/**
 * @brief Determina en qué mitad está un puntero. Asume que el puntero siempre
 *        apunta a un caracter váliod o al centinela de alguna mitad.
 */
static half _which_half(const char* p) { return (p >= buffer[B]) ? B : A; }

/*******************************
 *    FUNCIONES PÚBLICAS       *
 *******************************/

void input_open(const char* filename) {
    src = fopen(filename, "r");

    if (!src) {
        emit_error(0, FILE_OPEN_ERROR);
        exit(EXIT_FAILURE);
    }

    _load_half(A);

    lexeme_begin = buffer[A];
    forward = buffer[A];
}

void input_close() {
    if (src) {
        fclose(src);
        src = NULL;
    }
}

char input_getc() {
    char c = *forward;

    if (c == EOF) { // No puede aparecer
        half h = _which_half(forward);
        if (half_status[h].eof){
            forward ++;
            return EOF; // EOF real
        } 

        _load_half(1 - h);
        half_status[1 - h].used = true; // ahora sí se consumirá
        forward = buffer[1 - h];
        c = *forward;
    }
    forward++;
    half_status[_which_half(forward)].used = true; // Mitad como usada
    return (int)c;
}

char input_peek() {
    char c = *forward;

    if (c == EOF) { // No puede aparecer
        half h = _which_half(forward);
        if (half_status[h].eof) // EOF real
            return EOF;
        _load_half(1 - h);
        half_status[1 - h].used = false; // No la marcamos como usada
        c = buffer[1 - h][0];
    }
    return c;
}

void input_ungetc() {
    half h = _which_half(forward);
    if (forward == buffer[h]) { // Estamos al inicio de un buffer
        half prev = 1 - h;
        // Edge case: retroceder el primer caracter del archivo
        if (!half_status[prev].length) {
            emit_error(0, FILE_BEGIN);
            return;
        }
        // Forward al final de la mitad anterior (justo antes del centinela)
        forward = buffer[prev] + half_status[prev].length;
    } else { // Retrocedemos con normalidad al no ser el inicio
        forward--;
    }
}

void input_advance() { lexeme_begin = forward; }

const fragments* input_fragments() {

    frags.fr1 = lexeme_begin;

    // Mismo buffer -> un solo fragmento
    if (_which_half(lexeme_begin) == _which_half(forward)) {
        frags.len1 = (size_t)(forward - lexeme_begin);
        frags.fr2 = NULL;
        frags.len2 = 0;
    } else {

        frags.len1 =
            (size_t)(BUFFER_SIZE -
                     (lexeme_begin - buffer[_which_half(lexeme_begin)]));

        // Edge case: forward está en el inicio -> un solo fragmento
        if (forward == buffer[_which_half(forward)]) {
            frags.fr2 = NULL;
            frags.len2 = 0;
        } else {
            frags.fr2 = buffer[1 - _which_half(lexeme_begin)];
            frags.len2 = (size_t)(forward - buffer[1 - _which_half(forward)]);
        }
    }
    return &frags;
}