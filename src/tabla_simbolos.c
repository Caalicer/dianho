#include "tabla_simbolos.h"
#include "avl.h"
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
    printf("\t <%d, %s>\n", e.lexical_token, e.lexeme);
}

/**
 * @brief Compara dos elementos (lexemas) del AVL.
 * @param e1 Primer elemento
 * @param e2 Segundo elemento
 * @return Valor de comparación
 */
int _elemcmp(element* e1, element* e2) {
    return strcmp(e1->lexeme, e2->lexeme);
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

void symtab_print() {
    if (!table)
        return;
    printf("\n ** TABLA DE SÍMBOLOS **\n");
    printf("\t<lexical_token, lexeme>\n");
    avl_traverse(table, INORDER, _print_element);
}