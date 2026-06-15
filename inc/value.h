#pragma once

/**
 * @file value.h
 * @brief Definición de la estructura para representar valores en tiempo de
 * ejecución.
 **/

/// @brief Tipos de datos soportados
typedef enum {
    VALUE_INT,
    VALUE_DOUBLE,
    VALUE_BOOL,
    VALUE_STRING,
    VALUE_VOID,
    VALUE_ERROR
} value_type;

/**
 * @brief Representa un valor en tiempo de ejecución.
 *
 * Patrón de tagged union para representar los diferentes tipos
 */
typedef struct value {
    value_type type; ///< Tag del tipo de valor
    union {
        long long i;
        double d;
        int b;
        char* s;
    } as;
} value;

int value_type_is_numeric(value_type t);

/**
 * @name Makers para los diferentes tipos
 * @{
 */
value value_from_int(long long i);
value value_from_double(double d);
value value_from_bool(int b);
value value_from_string(const char* s);
value value_from_void(void);
value value_from_error(void);
/** @} */

/**
 * @brief Liberar memoria asociada a un valor, si aplica.
 *
 * @param v Referencia al valor.
 */
void value_free(value* v);

/**
 * @brief Copia una estructura src a dst, realizando nuevas reservas si es
 * necesario
 *
 * @param dst Referencia al valor destino, que será modificado
 * @param src Referencia al valor fuente, que no será modificaco.
 * @return int 1 si la copia fue exitosa, 0 si hubo un error.
 */
int value_copy(value* dst, const value* src);

/**
 * @brief Imprime el valor formateado según su tipo. No añade saltos de línea ni
 * retornos de carro.
 *
 * @param v Referencia al valor a imprimir.
 */
void value_print(const value* v);