#pragma once

#include <stddef.h>

typedef struct eval_env eval_env;
typedef struct ast_node ast_node;
typedef struct value value;

/// @brief Tipos de nodos del AST
typedef enum {
    AST_LITERAL,
    AST_IDENTIFIER,
    AST_UNARY,
    AST_BINARY,
    AST_CALL,
    AST_IF
} ast_node_type;

/// @brief Unary OPerations
typedef enum { UOP_NEG, UOP_NOT } ast_unary_op;

/// @brief Binary OPerations
typedef enum {
    BOP_ADD,     ///< Suma
    BOP_SUB,     ///< Resta
    BOP_MUL,     ///< Multiplicacion
    BOP_DIV,     ///< Division
    BOP_MOD,     ///< Modulo
    BOP_POW,     ///< Potencia
    BOP_GT,      ///< Mayor que
    BOP_GE,      ///< Mayor o igual que
    BOP_LT,      ///< Menor que
    BOP_LE,      ///< Menor o igual que
    BOP_EQ,      ///< Igual que
    BOP_NE,      ///< Diferente de
    BOP_AND,     ///< AND lógico
    BOP_OR,      ///< OR lógico
    BOP_BIT_AND, ///< AND bit a bit
    BOP_BIT_OR,  ///< OR bit a bit
    BOP_BIT_XOR, ///< XOR bit a bit
    BOP_LSHIFT,  ///< Desplazamiento a la izquierda
    BOP_RSHIFT   ///< Desplazamiento a la derecha
} ast_binary_op;

/**
 * @brief Crea un nuevo nodo literal
 * 
 * @param v Valor en tiempo de ejecución
 * @param line Línea en la fuente donde se encuentra el nodo
 * @param column Columna en la fuente donde se encuentra el nodo
 * @return ast_node* 
 */
ast_node* ast_new_literal(value v, int line, int column);

/**
 * @brief Crea un nuevo nodo identificador
 * 
 * @param name Nombre del identificador
 * @param line Línea en la fuente donde se encuentra el nodo
 * @param column Columna en la fuente donde se encuentra el nodo
 * @return ast_node* 
 */
ast_node* ast_new_identifier(char* name, int line, int column);

/**
 * @brief Crea un nuevo nodo unario
 * 
 * @param op Operación unaria
 * @param operand Operando
 * @param line Línea en la fuente donde se encuentra el nodo
 * @param column Columna en la fuente donde se encuentra el nodo
 * @return ast_node* 
 */
ast_node* ast_new_unary(ast_unary_op op, ast_node* operand, int line,
                        int column);

/**
 * @brief Crea un nuevo nodo binario
 * 
 * @param op Operación binaria
 * @param left Operando izquierdo
 * @param right Operando derecho
 * @param line Línea en la fuente donde se encuentra el nodo
 * @param column Columna en la fuente donde se encuentra el nodo
 * @return ast_node* 
 */
ast_node* ast_new_binary(ast_binary_op op, ast_node* left, ast_node* right,
                         int line, int column);

/**
 * @brief Crea un nuevo nodo de llamada a función
 * 
 * @param name Nombre de la función
 * @param args Argumentos de la llamada
 * @param argc Número de argumentos
 * @param line Línea en la fuente donde se encuentra el nodo
 * @param column Columna en la fuente donde se encuentra el nodo
 * @return ast_node* 
 */
ast_node* ast_new_call(char* name, ast_node** args, size_t argc, int line,
                       int column);

/**
 * @brief Crea un nuevo nodo if
 * 
 * @param cond Condición
 * @param then_branch Rama then
 * @param else_branch Rama else
 * @param line Línea en la fuente donde se encuentra el nodo
 * @param column Columna en la fuente donde se encuentra el nodo
 * @return ast_node* 
 */
ast_node* ast_new_if(ast_node* cond, ast_node* then_branch,
                     ast_node* else_branch, int line, int column);

/**
 * @brief Libera la memoria asociada a un nodo AST y sus hijos.
 * 
 * @param node Referenia al nodo AST a liberar.
 */
void ast_free(ast_node* node);

/**
 * @brief Evalúa un nodo AST y almacena el resultado en un valor.
 * 
 * @param node Nodo AST a evaluar.
 * @param out Puntero al valor donde almacenar el resultado.
 * @param env Entorno de evaluación.
 * @return int Código de error (0 si no hay errores).
 */
int ast_eval(const ast_node* node, value* out, const eval_env* env);
