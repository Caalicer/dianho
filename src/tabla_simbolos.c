#include "tabla_simbolos.h"
#include "avl.h"
#include "definiciones.h"
#include <stdio.h>
#include <stdlib.h>

static AVLTree* table = NULL; // tabla de símbolos opaca

/**
 * @brief Libera la memoria asociada a un elemento (lexema) del AVL.
 * @param e Elemento a librar
 */
void _free_element(element* e) { free(e->lexeme); }

/**
 * @brief Imprime un elemento (lexema) del AVL.
 * @param e Elemento a imprimir
 */
void _print_element(element e) {
    print_lexeme((lexeme) e);
}

/**
 * @brief Compara dos elementos (lexemas) del AVL.
 * @param e1 Primer elemento
 * @param e2 Segundo elemento
 * @return Valor de comparación
 */
int _elemcmp(const void* e1, const element* e2) {
    return strcmp(((element*)e1)->lexeme, e2->lexeme);
}

int _fragcmp_impl(const fragments* frag, const element* elem) {
    const char* str = elem->lexeme;
    // Comparar fr1
    for (size_t i = 0; i < frag->len1; i++) {
        if (*str == '\0')
            return 1; // str se agotó, frag es mayor
        int diff = (unsigned char)frag->fr1[i] - (unsigned char)*str++;
        if (diff != 0)
            return diff;
    }

    // Comparar fr2 si existe
    if (frag->fr2 != NULL) {
        for (size_t i = 0; i < frag->len2; i++) {
            if (*str == '\0')
                return 1; // str se agotó, frag es mayor
            int diff = (unsigned char)frag->fr2[i] - (unsigned char)*str++;
            if (diff != 0)
                return diff;
        }
    }

    // Los fragmentos se agotaron, ver si str también
    return -(unsigned char)*str; // 0 si str termina aquí, <0 si str es mayor
}

int _fragcmp(const void* frag, const element* e) {
    return _fragcmp_impl((const fragments*)frag, e);
}

void symtab_init() {

    if (table != NULL) // Ya existe
        return;

    table = avl_create(_elemcmp, _free_element);

    static lexeme keywords[] = {
        {IMPORT, "import"},   //
        {DOUBLE, "double"},   //
        {VOID, "void"},       //
        {INT, "int"},         //
        {WHILE, "while"},     //
        {FOREACH, "foreach"}, //
        {CAST, "cast"}        //
    };

    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        lexeme temp = keywords[i];
        // Para evitar problemas en symtab_terminate al usar _free_element
        temp.lexeme = strdup(temp.lexeme);
        avl_insert(table, &temp);
    }
}

void symtab_terminate() {
    if (!table)
        return;
    avl_destroy(table);
    table = NULL;
}

lexeme* symtab_intern(lexeme* lexeme) {
    if (!table)
        return NULL;
    // Buscar
    element* found = avl_search(table, lexeme);
    if (found) { // Existe: liberar el lexema temporal y devolver el encontrado
        free(lexeme->lexeme);
        free(lexeme);
        return found;
    }
    // No existe -> insertar
    element* inserted = avl_insert(table, lexeme);
    if (!inserted) { // Error en la inserción -> no eliminar el lexema temporal
        return NULL;
    }

    return inserted;
}

lexeme* symtab_fragments_lookup(const fragments* token_fragments) {
    if (!table)
        return NULL;
    return avl_fragment_search(table, token_fragments, _fragcmp);
}

void symtab_print() {
    if (!table)
        return;
    printf("\n ** TABLA DE SÍMBOLOS **\n");
    printf("\t<lexical_token, lexeme>\n");
    avl_traverse(table, INORDER, _print_element);
}