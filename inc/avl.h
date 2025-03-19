#pragma once

/**
 * @file avl.h
 * @brief Definición de la API para un Árbol AVL.
 *
Proporciona funciones para la manipulación de un Árbol AVL.

- Manipulación del árbol

 **/

#include "definiciones.h"
#include <stddef.h>

/**
 * @typedef key
 * @brief Tipo de clave usada para insertar en el AVLTree.
 **/
typedef char* key;

/**
 * @typedef element
 * @brief Tipo de información a almacenar en los nodos.
 **/
typedef lexeme element;

/**
 * @struct AVLTree
 * @brief Estructura opaca que representa un Árbol AVL.
 **/
typedef struct AVLTree AVLTree;

/**
 * @brief Función auxiliar para extraer la clave del elemento a insertar.
 * @return Clave para insertar en el AVLTree.
 **/
key _clave_elem(element* data) {
    return data->lexeme;
}

/**
 * @brief Crea un nuevo AVLTree vacío.
 * @return Puntero al AVLTree. NULL si falla la asignación de memoria.
 **/
AVLTree* avl_create();

/**
 * @brief Destruye un AVLTree y libera toda la memoria asociada.
 * @param tree Puntero al AVLTree a destruir.
 **/
void avl_destroy(AVLTree* tree);

/**
 * @brief Inserta un nodo con una clave en el Árbol AVL.
 * @param tree Puntero al AVLTree donde se insertará la clave.
 * @param data Nodo a insertar en el árbol.
 * @retval  0 La inserción fue exitosa.
 * @retval -1 Hubo un error en la inserción.
 */
int avl_insert(AVLTree* tree, element data);

/**
 * @brief Elimina un nodo con la clave dada del Árbol AVL.
 * @param tree Puntero al AVLTree de donde se eliminará la clave.
 * @param data Nodo a eliminar en el árbol.
 * @retval  0 La eliminación fue exitosa.
 * @retval -1 La clave no se encontró.
 **/
int avl_remove(AVLTree* tree, element data);

/**
 * @brief Busca una clave en el Árbol AVL.
 * @param tree Puntero al AVLTree en el que se realizará la búsqueda.
 * @param data Nodo a buscar en el árbol.
 * @retval 1 La clave está en el árbol.
 * @retval 0 No se encontró la clave.
 **/
int avl_search(const AVLTree* tree, element data);

/**
 * @brief Obtiene el número de nodos en el Árbol AVL.
 * @param tree Puntero AVLTree.
 * @return Número de nodos en el árbol.
 **/
size_t avl_size(const AVLTree* tree);

/**
 * @brief Obtiene la altura del Árbol AVL.
 * @param tree Puntero al AVLTree.
 * @return La altura del árbol. -1 si está vacío.
 **/
int avl_height(const AVLTree* tree);

/**
 * @brief Recorre el Árbol AVL en inorden y ejecuta una función callback
 *        en cada nodo.
 * @param tree Puntero al AVLTree a recorrer.
 * @param callback Función que se ejecutará en cada nodo del árbol.
 **/
void avl_traverse_inorder(const AVLTree* tree, void (*callback)(element));

/**
 * @brief Recorre el Árbol AVL en preorden y ejecuta una función callback en
 *cada nodo.
 * @param tree Puntero al AVLTree a recorrer.
 * @param callback Función que se ejecutará en cada nodo del árbol.
 **/
void avl_traverse_preorder(const AVLTree* tree, void (*callback)(element));

/**
 * @brief Recorre el Árbol AVL en postorden y ejecuta una función callback en
 *cada nodo.
 * @param tree Puntero al AVLTree a recorrer.
 * @param callback Función que se ejecutará en cada nodo del árbol.
 **/
void avl_traverse_postorder(const AVLTree* tree, void (*callback)(element));
