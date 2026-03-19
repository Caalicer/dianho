#pragma once

/**
 * @file avl.h
 * @brief Definición de las funciones públicas para un Árbol AVL.
 *
 * Proporciona funciones para la manipulación de un Árbol AVL.
 *
 * - Manipulación del árbol
 *
 * >[!Warning] elemcmp
 * > Esta función debe compararse como strcmp y devolver un entero de la forma:
 * > - 0, si e1 y e2 son iguales.
 * >     - Valor *negativo* si e1 es menor que e2.
 * >     - Valor *positivo* si e1 es mayor que e2.
 **/

#include "definiciones.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREORDER 0
#define INORDER 1
#define POSTORDER 2

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
 * @brief Crea un nuevo AVLTree vacío.
 * @param elemcmp Función de comparación de elementos
 * @param free_element Función callback para liberar la memoria del elemento
 * @return Puntero al AVLTree. NULL si falla la asignación de memoria.
 **/
AVLTree* avl_create(int (*elemcmp)(const void* e1, const element* e2),
                    void (*free_element)(element* e));

/**
 * @brief Destruye un AVLTree y libera toda la memoria asociada.
 * @param tree Puntero al AVLTree a destruir.
 **/
void avl_destroy(AVLTree* tree);

/**
 * @brief Inserta un nodo con una clave en el Árbol AVL.
 * @param tree Puntero al AVLTree donde se insertará la clave.
 * @param data Puntero al elemento a insertar en el árbol.
 * @retval  e Puntero al elemento insertado.
 * @retval  NULL Error en la inserción.
 */
element* avl_insert(AVLTree* tree, element* data);

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
 * @retval e Puntero al elemento encontrado.
 * @retval NULL No se encontró la clave.
 **/
element* avl_search(const AVLTree* tree, element* data);

/**
 * @brief Busca mediante los fragmentos de una clave en el Árbol AVL
 *
 * @param tree Puntero al AVLTree en el que se realizará la búsqueda.
 * @param frag Fragmentos a buscar
 * @param fragcmp Función de comparación de los fragmentos con un element
 * @return element*
 */
element* avl_fragment_search(const AVLTree* tree, const fragments* frag,
                             int (*fragcmp)(const void* frag,
                                            const element* e));

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
 * @brief Recorre el Árbol AVL en preorder, inorder o postorden y ejecuta una
 *        función callback en cada elemento almacenado en el árbol.
 *
 * @param tree  Puntero al AVLTree a recorrer.
 * @param order Orden del recorrido:
 *        - `PREORDER`  (0): Nodo → Izquierda → Derecha
 *        - `INORDER`   (1): Izquierda → Nodo → Derecha
 *        - `POSTORDER` (2): Izquierda → Derecha → Nodo
 * @param callback Función que se ejecutará en cada nodo del árbol.
 **/
void avl_traverse(AVLTree* tree, int order, void (*callback)(element));
