Declaramos los estados transiciones y tablas de la forma

```c
typedef enum {
    S_START,
    S_NUMBER,
    S_ACCEPT,
    STATE_COUNT
} State;

typedef enum {
    SY_DIGIT,
    SY_OTHER,
    SYMBOL_COUNT
    } Symbol;
    
int transitions[STATE_COUNT][SYMBOL_COUNT] =
    { [S_START] = {S_NUMBER, S_START},
      [S_NUMBER] = {S_NUMBER, S_ACCEPT},
      [S_ACCEPT] = {S_ACCEPT, S_ACCEPT}};

```