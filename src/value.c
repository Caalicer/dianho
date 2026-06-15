#include "value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definiciones.h"

int value_type_is_numeric(value_type t) {
    return t == VALUE_INT || t == VALUE_DOUBLE;
}

value value_from_int(long long i) {
    return (value){.type = VALUE_INT, .as.i = i};
}

value value_from_double(double d) {
    return (value){.type = VALUE_DOUBLE, .as.d = d};
}

value value_from_bool(int b) {
    return (value){.type = VALUE_BOOL, .as.b = !!b};
}

value value_from_void(void) { return (value){.type = VALUE_VOID}; }

value value_from_error(void) { return (value){.type = VALUE_ERROR}; }

value value_from_string(const char* s) {
    value v;
    v.type = VALUE_STRING;
    v.as.s = s ? strdup(s) : strdup("");
    if (!v.as.s) {
        v.type = VALUE_ERROR;
    }
    return v;
}

void value_free(value* v) {
    if (!v) {
        return;
    }
    if (v->type == VALUE_STRING) {
        free(v->as.s);
        v->as.s = NULL;
    }
    v->type = VALUE_VOID;
}

int value_copy(value* dst, const value* src) {
    if (!dst || !src) {
        return 0;
    }
    dst->type = src->type;
    if (src->type == VALUE_STRING) {
        dst->as.s = src->as.s ? strdup(src->as.s) : strdup("");
        return dst->as.s != NULL;
    }
    dst->as = src->as;
    return 1;
}

void value_print(const value* v) {
    if (!v) {
        return;
    }
    switch (v->type) {
        case VALUE_INT:
            printf("%lld", v->as.i);
            break;
        case VALUE_DOUBLE:
            printf("%g", v->as.d);
            break;
        case VALUE_BOOL:
            printf(v->as.b ? "true" : "false");
            break;
        case VALUE_STRING:
            printf("\"%s\"", v->as.s ? v->as.s : "");
            break;
        case VALUE_VOID:
            printf("void");
            break;
        case VALUE_ERROR:
        default:
            printf(C_RED "<error>" C_RESET);
            break;
    }
}