/**
 * @file avl.c
 *
 * @brief
 *
 */

#include "avl.h"

// typedef struct node node;

/**
 * @struct Nodo del árbol AVL
 * @brief Estructura de cada nodo del árbol AVL.
 *         - Punteros a nodos hijos y al nodo padre
 *         - Información interna del nodo: altura y factor de equilibrio
 *         - Información almacenada
 */
typedef struct node {
    element data;        /**< Información almacenada    */
    int balance;         /**< Factor de equilibrio      */
    int height;          /**< Altura en el árbol        */
    struct node* parent; /**< Puntero al nodo padre     */
    struct node* left;   /**< Puntero al hijo izquierdo */
    struct node* right;  /**< Puntero al hijo derecho   */
} node;

/**
 * @struct Árbol AVL
 * @brief Estructuración del árbol AVL
 */
struct AVLTree {
    node* root;  /**< Puntero al nodo raíz del árbol */
    size_t size; /**< Tamaño del árbol */
    void (*free_element)(
        element* e); /**< Función callback para liberar el elemento */
    int (*elemcmp)(
        const void* e1,
        const element* e2); /**< Función de comparación de elementos */
};

/**********************************
 * Declaración funciones privadas *
 **********************************/

/**
 * @brief Crea un nuevo nodo con los datos proporcionados.
 *
 * @param data Datos a almacenar en el nodo.
 * @return Puntero al nuevo nodo. NULL si falla la asignación de memoria.
 */
static node* _create_node(element* data);

/**
 * @brief Libera la memoria ocupada por un nodo del árbol AVL y todos sus
 * descendientes.
 *
 * @param node Puntero al nodo que será liberado. Si es NULL, la función no
 *             hace nada.
 * @param free_element Función callback para liberar el elemento almacenado en
 *                     el nodo.
 */
static void _free_node(node* node, void (*free_element)(element* e));

/**
 * @brief Recorrido preorden sobre el subárbol con raíz en el nodo actual,
 *        aplicándose un callback sobre cada nodo.
 *
 * Recorrido: Nodo -> Subárbol izquierdo -> Subárbol derecho
 *
 * @param nodo Puntero al nodo actual.
 * @param callback Función que se ejecutará en cada nodo.
 * @param ctx Contexto adicional que se pasa a la función callback.
 */
static void _preorder(node* nodo, void (*callback)(void*, void*), void* ctx);

/**
 * @brief Recorrido inorden sobre el subárbol con raíz en el nodo actual,
 *        aplicándose un callback sobre cada nodo.
 *
 * Recorrido: Subárbol izquierdo -> Nodo -> Subárbol derecho
 *
 * @param nodo Puntero al nodo actual.
 * @param callback Función que se ejecutará en cada nodo.
 * @param ctx Contexto adicional que se pasa a la función callback.
 */
static void _inorder(node* nodo, void (*callback)(void*, void*), void* ctx);

/**
 * @brief Recorrido postorden sobre el subárbol con raíz en el nodo actual,
 *        aplicándose un callback sobre cada nodo.
 *
 * Recorrido: Subárbol izquierdo -> Subárbol derecho -> Nodo
 *
 * @param nodo Puntero al nodo actual.
 * @param callback Función que se ejecutará en cada nodo.
 * @param ctx Contexto adicional que se pasa a la función callback.
 */
static void _postorder(node* nodo, void (*callback)(void*, void*), void* ctx);

/**
 * @brief Adaptador que convierte la función de recorrido genérica en una que
 * recibe `element`.
 *
 * @param nodo Puntero al nodo actual.
 * @param ctx Contexto adicional. sSe espera un puntero a una
 *            función `void (*)(element)`).
 */
static void _element_adapter(void* nodo, void* ctx);

/**
 * @brief
 *
 * @param tree Puntero al AVLTree donde
 * @param n
 */
static void _rebalance(AVLTree* tree, node* n);

static node* _rotate_left(AVLTree* tree, node* n);

static node* _rotate_right(AVLTree* tree, node* n);

static void _update_height_balance(node* n);

/**
 * @brief Buscar un nodo con la clave dada en el Árbol AVL.
 *
 * @param tree Puntero al AVLTree en el que se realizará la búsqueda.
 * @param data Elemento a buscar.
 * @param cmp Función de comparación personalizada. Si es NULL, se usará la
 *            función de comparación del árbol.
 * @param last Parametro de salida. Nodo padre del nodo encontrado o del último
 *             nodo visitado si no se encuentra.
 * @return node* Puntero al nodo encontrado, o NULL si no se encuentra.
 */
static node* _search(const AVLTree* tree, void* data,
                     int (*cmp)(const void*, const element*), node** last);

static node* _find_min(node* n);

/*************************************
 * Implementación funciones públicas *
 *************************************/

AVLTree* avl_create(int (*elemcmp)(const void* e1, const element* e2),
                    void (*free_element)(element* e)) {

    AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));
    if (!tree) {
        return NULL;
    }
    tree->root = NULL;
    tree->size = 0;
    tree->free_element = free_element;
    tree->elemcmp = elemcmp;
    return tree;
}

void avl_destroy(AVLTree* tree) {

    if (!tree) {
        return;
    }

    _free_node(tree->root, tree->free_element);

    free(tree);
}

element* avl_insert(AVLTree* tree, element* data) {

    if (!tree) {
        return NULL;
    }

    node* new_node = _create_node(data);
    if (!new_node) {
        return NULL;
    }

    // Caso especial: árbol vacío
    if (!tree->root) {
        tree->root = new_node;
        tree->size = 1;
        return &(new_node->data);
    }

    node* parent = NULL;

    if (!_search(tree, data, NULL, &parent)) {
        new_node->parent = parent;

        if (tree->elemcmp(data, &(parent->data)) < 0) {
            parent->left = new_node;
        } else {
            parent->right = new_node;
        }

    } else { // El elemento ya existe en el árbol
        free(new_node);
        return NULL;
    }

    // Actualizar altura y rebalancear desde el nodo insertado hacia arriba
    _rebalance(tree, new_node);

    tree->size++;
    return &(new_node->data);
}

// todo revisar
int avl_remove(AVLTree* tree, element data) {
    if (!tree || !tree->root) {
        return -1;
    }

    node* target = _search(tree, &data, NULL, NULL);
    if (!target) {
        return -1; // El elemento no existe en el árbol
    }

    node* parent = target->parent;
    node* replacement = NULL;
    node* rebalance_start = NULL;

    // Caso 1: El nodo a eliminar no tiene hijos o tiene solo un hijo
    if (!target->left || !target->right) {
        // Obtener el hijo que no es NULL (si existe)
        replacement = target->left ? target->left : target->right;

        // Actualizar el enlace del padre al hijo
        if (parent) {
            if (parent->left == target) {
                parent->left = replacement;
            } else {
                parent->right = replacement;
            }
            rebalance_start = parent;
        } else {
            tree->root = replacement; // El nodo a eliminar es la raíz
        }

        // Actualizar el enlace del hijo al padre
        if (replacement) {
            replacement->parent = parent;
        }
    }
    // Caso 2: El nodo a eliminar tiene dos hijos
    else {
        // Encontrar el sucesor en orden (mínimo del subárbol derecho)
        replacement = _find_min(target->right);

        // Si el sucesor no es el hijo derecho inmediato
        if (replacement->parent != target) {
            // Reemplazar el sucesor con su hijo derecho
            if (replacement->right) {
                replacement->right->parent = replacement->parent;
            }
            replacement->parent->left = replacement->right;

            // El sucesor adopta el hijo derecho del nodo a eliminar
            replacement->right = target->right;
            target->right->parent = replacement;

            rebalance_start = replacement->parent;
        } else {
            rebalance_start = replacement;
        }

        // Actualizar el enlace del padre al sucesor
        if (parent) {
            if (parent->left == target) {
                parent->left = replacement;
            } else {
                parent->right = replacement;
            }
        } else {
            tree->root = replacement;
        }

        // El sucesor adopta el hijo izquierdo del nodo a eliminar
        replacement->left = target->left;
        target->left->parent = replacement;
        replacement->parent = parent;
    }

    tree->free_element(&(target->data));
    free(target);

    // Rebalancear el árbol desde el punto de rebalanceo
    if (rebalance_start) {
        _rebalance(tree, rebalance_start);
    }

    tree->size--;
    return 0;
}

element* avl_search(const AVLTree* tree, element* data) {
    node* found = _search(tree, data, NULL, NULL);
    return found ? &(found->data) : NULL;
}

element* avl_fragment_search(const AVLTree* tree, const fragments* frag,
                             int (*fragcmp)(const void* frag,
                                            const element* e)) {
    node* found = _search(tree, (void *)frag, fragcmp, NULL);
    return found ? &(found->data) : NULL;
}

static node* _find_min(node* n) {
    if (!n) {
        return NULL;
    }

    node* current = n;

    while (current->left) {
        current = current->left;
    }

    return current;
}

size_t avl_size(const AVLTree* tree) { return tree->size; }

int avl_height(const AVLTree* tree) { return tree->root->height; }

void avl_traverse(AVLTree* tree, int order, void (*callback)(element)) {

    if (!tree) {
        return;
    }

    switch (order) {
        case PREORDER:
            _preorder(tree->root, _element_adapter, (void*)callback);
            break;
        case INORDER:
            _inorder(tree->root, _element_adapter, (void*)callback);
            break;
        case POSTORDER:
            _postorder(tree->root, _element_adapter, (void*)callback);
            break;
        default:
            fprintf(stderr, "Error: Orden de recorrido inválido\n");
            break;
    }
}

/*************************************
 * Implementacion funciones privadas *
 *************************************/

static node* _create_node(element* data) {

    node* new_node = (node*)malloc(sizeof(node));
    if (!new_node) {
        return NULL;
    }

    new_node->data = *data;
    new_node->height = 0;
    new_node->balance = 0;
    new_node->parent = NULL;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

static void _rebalance(AVLTree* tree, node* n) {
    if (!n) {
        return;
    }

    node* current = n;

    // Subir por el árbol actualizando alturas y rebalanceando
    while (current) {
        _update_height_balance(current);

        // Verificar si el nodo está desbalanceado
        if (current->balance < -1) {
            // Caso Izquierda-Derecha
            if (current->left && current->left->balance > 0) {
                _rotate_left(tree, current->left);
            }
            // Caso Izquierda-Izquierda
            current = _rotate_right(tree, current);
        } else if (current->balance > 1) {
            // Caso Derecha-Izquierda
            if (current->right && current->right->balance < 0) {
                _rotate_right(tree, current->right);
            }
            // Caso Derecha-Derecha
            current = _rotate_left(tree, current);
        }

        // Subir al padre
        current = current->parent;
    }
}

static node* _rotate_left(AVLTree* tree, node* n) {
    if (!n) {
        return NULL;
    }

    node* pivot = n->right;
    node* parent = n->parent;

    // Rotación
    n->right = pivot->left;
    pivot->left = n;
    n->parent = pivot;

    // Actualizar padre
    if (n->right) {
        n->right->parent = n;
    }

    // Actualizar padre del pivot
    pivot->parent = parent;

    // Actualizar hijo del padre
    if (parent) {
        if (parent->left == n) {
            parent->left = pivot;
        } else {
            parent->right = pivot;
        }
    } else {
        tree->root = pivot;
    }

    // Actualizar alturas y factores de equilibrio
    _update_height_balance(n);
    _update_height_balance(pivot);

    return pivot;
}

static node* _rotate_right(AVLTree* tree, node* n) {
    if (!n) {
        return NULL;
    }

    node* pivot = n->left;
    node* parent = n->parent;

    // Rotación
    n->left = pivot->right;
    pivot->right = n;
    n->parent = pivot;

    // Actualizar padre
    if (n->left) {
        n->left->parent = n;
    }

    // Actualizar padre del pivot
    pivot->parent = parent;

    // Actualizar hijo del padre
    if (parent) {
        if (parent->left == n) {
            parent->left = pivot;
        } else {
            parent->right = pivot;
        }
    } else {
        tree->root = pivot;
    }

    // Actualizar alturas y factores de equilibrio
    _update_height_balance(n);
    _update_height_balance(pivot);

    return pivot;
}

static void _update_height_balance(node* n) {
    if (!n) {
        return;
    }

    int left_height = n->left ? n->left->height : -1;
    int right_height = n->right ? n->right->height : -1;

    n->height = 1 + (left_height > right_height ? left_height : right_height);
    n->balance = right_height - left_height;
}

static node* _search(const AVLTree* tree, void* data,
                     int (*cmp)(const void*, const element*), node** last) {
    if (!tree || !tree->root) {
        return NULL;
    }

    node* current = tree->root;
    int comparison;

    while (current) {
        if (last)
            *last = current;
        comparison = cmp == NULL ? tree->elemcmp(data, &(current->data))
                                 : cmp(data, &(current->data));
        if (comparison == 0) {
            return current; // Elemento encontrado
        } else if (comparison < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL; // Elemento no encontrado
}

static void _free_node(node* node, void (*free_element)(element* e)) {

    if (!node) {
        return;
    }

    // Postorder
    _free_node(node->left, free_element);
    _free_node(node->right, free_element);

    free_element(&(node->data)); // liberar elemento
    free(node);                  // liberar nodo
}

static void _preorder(node* nodo, void (*callback)(void*, void*), void* ctx) {

    if (!nodo) {
        return;
    }

    callback(nodo, ctx);                   // Nodo actual
    _preorder(nodo->left, callback, ctx);  // Subárbol izquierdo
    _preorder(nodo->right, callback, ctx); // Subárbol derecho
}

static void _inorder(node* nodo, void (*callback)(void*, void*), void* ctx) {

    if (!nodo) {
        return;
    }

    _inorder(nodo->left, callback, ctx);  // Subárbol izquierdo
    callback(nodo, ctx);                  // Nodo actual
    _inorder(nodo->right, callback, ctx); // Subárbol derecho
}

static void _postorder(node* nodo, void (*callback)(void*, void*), void* ctx) {

    if (!nodo) {
        return;
    }

    _postorder(nodo->left, callback, ctx);  // Subárbol izquierdo
    _postorder(nodo->right, callback, ctx); // Subárbol derecho
    callback(nodo, ctx);                    // Nodo actual
}

static void _element_adapter(void* nodo, void* ctx) {
    void (*callback)(element) = (void (*)(element))ctx;
    callback(((node*)nodo)->data);
}