/**
 * @file avl.c
 *
 * @brief
 *
 */

#include "avl.h"
#include <stdlib.h>

/**
 * @struct Nodo del árbol AVL
 * @brief Estructura de cada nodo del árbol AVL.
 * - Punteros a nodos hijos y al nodo padre
 * - Información interna del nodo: altura y factor de equilibrio
 * - Información almacenada
 */
typedef struct {
    element data;        /**< Información almacenada    */
    int balance;         /**< Factor de equilibrio      */
    int height;          /**< Altura en el árbol        */
    struct node* parent; /**< Puntero al nodo padre     */
    struct node* left;   /**< Puntero al hijo izquierdo */
    struct node* right;  /**< Punteor al hijo derecho   */
} node;

/**
 * @struct Árbol AVL
 * @brief Estructuración del árbol AVL
 */
struct AVLTree {
    node* root;  /**< Puntero al nodo raíz del árbol */
    size_t size; /**< Puntero al nodo */
};

/**
 * @def DEFINE_ADAPTER(name, type, field)
 * @brief Adaptador de una función con tipo específico en un callback genérico.
 *
 * Esta macro genera una función estática que actúa como un adapter para 
 * convertir un callback que recibe un tipo específico (`type`) en un
 * callback genérico que recibe `void*`.
 * 
 * Esto permite implementar algoritmos de recorrido que se desacoplen del tipo
 * de dato sobre el que se desee trabajar (node* o element) a más alto nivel.
 *
 * @param name  Nombre del adapter usado como nombre de la función generada.
 * @param type  Tipo de dato esperado por el callback.
 * @param field Nombre del campo en `node` que se pasará al callback.
 *              Si `field` es vacío, se pasa directamente el `node*`.
 *
 * @note La función generada tiene la firma:
 *       `static void name(void* nodo, void* ctx);`
 *       - `nodo`: puntero genérico (`void*`), que se castea a `node*`.
 *       - `ctx`: `void*` casteado a un puntero a función (`void (*)(type)`).
 *       - Llama al callback con el tipo correcto (`type`).
 *
 * @warning Esta macro asume que `nodo` es siempre un puntero válido a `node*`.
 *
 * @see avl_postorder
 */
 #define DEFINE_ADAPTER(name, type, field)         \
 static void name(void* nodo, void* ctx) {         \
     void (*callback)(type) = (void (*)(type))ctx; \
     callback(((node*)nodo)->field);               \
 }

DEFINE_ADAPTER(callback_element_adapter, element, data);
DEFINE_ADAPTER(callback_node_adapter, node*,);

/**************
 *API PÚBLICA
 **************/

AVLTree* avl_create() {
    AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}


void avl_destroy(AVLTree* tree) {
    
}

int avl_insert(AVLTree* tree, element data) { return 0; }

int avl_remove(AVLTree* tree, element data) { return 0; }

int avl_search(const AVLTree* tree, element data) { return 1; }

size_t avl_size(const AVLTree* tree) { return 0; }

int avl_height(const AVLTree* tree) { return 0; }

void avl_traverse_inorder(const AVLTree* tree, void (*callback)(element)) {}

void avl_traverse_preorder(const AVLTree* tree, void (*callback)(element)) {}

void avl_traverse_postorder(const AVLTree* tree, void (*callback)(element)) {}