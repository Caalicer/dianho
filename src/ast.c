#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "value.h"

/**
 * @brief Representación de un nodo del Abstract Syntax Tree (AST).
 */
struct ast_node {
    ast_node_type type; ///< Tag del tipo de nodo
    int line;           ///< Línea en la fuente donde se encuentra el nodo
    int column;         ///< Columna en la fuente donde se encuentra el nodo
    union {
        value literal;    ///< Valor del nodo
        char* identifier; ///< Nombre del identificador
        struct {
            ast_unary_op op;   ///< Operador unario
            ast_node* operand; ///< Operando del operador unario
        } unary;
        struct {
            ast_binary_op op; ///< Operador binario
            ast_node* left;   ///< Operando izquierdo del operador binario
            ast_node* right;  ///< Operando derecho del operador binario
        } binary;
        struct {
            char* name;      ///< Nombre de la función
            ast_node** args; ///< Argumentos de la función
            size_t arity;    ///< Aridad de la función
        } call;
        struct {
            ast_node* cond;        ///< Condicional
            ast_node* then_branch; ///< En caso de verdad
            ast_node* else_branch; ///< En caso de falsedad
        } if_expr;
    } data;
};

static ast_node* alloc_node(ast_node_type type, int line, int column) {
    ast_node* node = (ast_node*)calloc(1, sizeof(ast_node));
    if (!node) {
        // Majar error de memoria
        return NULL;
    }
    node->type = type;
    node->line = line;
    node->column = column;
    return node;
}

ast_node* ast_new_literal(value v, int line, int column) {
    ast_node* node = alloc_node(AST_LITERAL, line, column);
    if (!node) {
        value_free(&v);
        return NULL;
    }
    node->data.literal = v;
    return node;
}

ast_node* ast_new_identifier(char* name, int line, int column) {
    ast_node* node = alloc_node(AST_IDENTIFIER, line, column);
    if (!node) {
        free(name);
        return NULL;
    }
    node->data.identifier = name;
    return node;
}

ast_node* ast_new_unary(ast_unary_op op, ast_node* operand, int line,
                        int column) {
    ast_node* node = alloc_node(AST_UNARY, line, column);
    if (!node) {
        ast_free(operand);
        return NULL;
    }
    node->data.unary.op = op;
    node->data.unary.operand = operand;
    return node;
}

ast_node* ast_new_binary(ast_binary_op op, ast_node* left, ast_node* right,
                         int line, int column) {
    ast_node* node = alloc_node(AST_BINARY, line, column);
    if (!node) {
        ast_free(left);
        ast_free(right);
        return NULL;
    }
    node->data.binary.op = op;
    node->data.binary.left = left;
    node->data.binary.right = right;
    return node;
}

ast_node* ast_new_call(char* name, ast_node** args, size_t argc, int line,
                       int column) {
    ast_node* node = alloc_node(AST_CALL, line, column);
    size_t i;
    if (!node) {
        free(name);
        if (args) {
            for (i = 0; i < argc; ++i) {
                ast_free(args[i]);
            }
        }
        free(args);
        return NULL;
    }
    node->data.call.name = name;
    node->data.call.args = args;
    node->data.call.arity = argc;
    return node;
}

ast_node* ast_new_if(ast_node* cond, ast_node* then_branch,
                     ast_node* else_branch, int line, int column) {
    ast_node* node = alloc_node(AST_IF, line, column);
    if (!node) {
        ast_free(cond);
        ast_free(then_branch);
        ast_free(else_branch);
        return NULL;
    }
    node->data.if_expr.cond = cond;
    node->data.if_expr.then_branch = then_branch;
    node->data.if_expr.else_branch = else_branch;
    return node;
}

void ast_free(ast_node* node) {
    size_t i;
    if (!node) {
        return;
    }
    switch (node->type) {
        case AST_LITERAL:
            value_free(&node->data.literal);
            break;
        case AST_IDENTIFIER:
            free(node->data.identifier);
            break;
        case AST_UNARY:
            ast_free(node->data.unary.operand);
            break;
        case AST_BINARY:
            ast_free(node->data.binary.left);
            ast_free(node->data.binary.right);
            break;
        case AST_CALL:
            free(node->data.call.name);
            for (i = 0; i < node->data.call.arity; ++i) {
                ast_free(node->data.call.args[i]);
            }
            free(node->data.call.args);
            break;
        case AST_IF:
            ast_free(node->data.if_expr.cond);
            ast_free(node->data.if_expr.then_branch);
            ast_free(node->data.if_expr.else_branch);
            break;
        default:
            break;
    }
    free(node);
}
