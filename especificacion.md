# Intérprete matemático

## Índice

- [Intérprete matemático](#intérprete-matemático)
    - [Índice](#índice)
    - [Lenguaje](#lenguaje)
        - [Literales](#literales)
            - [Verificación de tipos](#verificación-de-tipos)
        - [Expresiones](#expresiones)
            - [Operadores](#operadores)
            - [Funciones](#funciones)
        - [Statements](#statements)
            - [Declaración de variables](#declaración-de-variables)
            - [Declaración de funciones](#declaración-de-funciones)
                - [Funciones externas](#funciones-externas)
                - [Sobre los `string`](#sobre-los-string)
    - [Entorno](#entorno)
        - [Gestión del entorno](#gestión-del-entorno)
        - [Utilidades](#utilidades)
    - [Librería Estándar](#librería-estándar)
        - [Constantes](#constantes)
    - [Gestión de errores](#gestión-de-errores)
        - [Errores](#errores)
        - [Warnings](#warnings)
    - [Por definir](#por-definir)

## Lenguaje

Lo que define el lenguaje en sí del intérprete. Las funcionalidades básicas que el usuario puede emplear para escribir las expresiones que se evaluarán

### Literales

Se soportan cuatro tipos de literales: `double`, `int`, `bool` y `string`. Los `double` representan los números de punto flotante; los `int` números enteros y los `bool` un valor booleano que puede ser `true` o `false`.

En caso de los `string`, deben ir explícitamente contenidos entre `"`. Por ejemplo, si escribimos `hola` sería un `ID`, mientras que `"hola"` sí sería un `string`.

#### Verificación de tipos

La verificación de tipos es dinámica, por lo que el propio intérprete infiere el tipo de dato en cada expresión. Existe, por tanto, una problemática que el intérprete tiene que gestionar: expresiones que contienen más de un tipo.

Cuando se mezclan literales del tipo `int` con el tipo `double` el intérprete asume el tipo general `double`, haciendo el cast `int` -> `double`. Esto lanzará un warning: `implicit conversion int → double`. Ver la sección [Warnings](#warnings).

Cuando una expresión contiene un `bool`, todos los elementos de dicha expresión deben ser también `bools`, o se lanzará un error: `expected bool`.

El caso de los `string` es particular, ya que es un tipo sin operadores y por tanto no hay reglas de composición. Entonces, si en cualquier expresión aparece más de un `string` a la vez se mostraría un error `unexpected string`. Como en `1 + "hola"` o en `"hola" "mundo"`. Para más información de errores ver la sección [Errores](#errores).

El caso de las funciones **definidas por el usuario** es particular. Como no está soportado la especificación de tipos ni para argumentos ni valores de retorno, la verificación de tipos se produciría al evaluar la expresión con los argumentos de entrada.

Las funciones **definidas por el usuario** son un caso particular. Al no disponer de anotaciones de tipo en sus parámetros ni en su valor de retorno la verificación de tipos se realiza de forma dinámica en tiempo de ejecución al evaluar el cuerpo expresión tras la substitución de argumentos. Este diseño implica un comportamiento polimórfico implícito, ya que una misma función puede ser aplicada con argumentos de distintos tipos siempre que las operaciones utilizadas en su cuerpo sean válidas para los valores proporcionados.

> [!NOTE]
> Una mejora a este sistema podría ser la incorporación de un sistema de polimorfismo paramétrico o inferencia de tipos estática, similar al de lenguajes como `Haskell` o `Rust`, con el objetivo de trasladar la verificación de tipos a una fase previa a la ejecución. En lenguajes compilados es especialmente interesante, ya que traslada los errores de tipado a tiempo de compilación.

### Expresiones

Definimos las expresiones como una estructura sintáctica que se evalúa a un resultado que devuelve. Por ejemplo `1 + 2` es una expresión cuyo resultado final es `3`. Diferenciamos dos tipos principales de expresiones en función del tipo del resultado que devuelven:

1. Expresiones booleanas: cuyo resultado es un literal `bool`.
2. Expresiones numéricas: cuyo resultado es un literal `int` o `double`.
3. Expresiones string: cuyo resultado es un literal `string`.

Nótese, por ejemplo, que si escribimos un literal individual de la forma `1` o `1.5` o `true` el intérprete imprimirá ese número por pantalla. Esto es así porque una expresión puede estar formada únicamente por un literal.

> [!Caution] Sobre los `string`
>
> Como no se implementan operadores sobre `string`, cualquier expresión
> string está conformada por un solo literal `string`. Cualquier otra situación
> generaría un fallo.

#### Operadores

Los operadores no son expresiones como tal, pero viven dentro de estas. Tienen unos valores de entrada y producen una salida.

- **Unarios**: solo operan sobre una entrada
    - `-a` Negación de un literal `int` o `double`
    - `!a` Negación lógica de un `bool`
- **Binarios**: operan sobre dos entradas
    - **Matemáticos**: se aplican sobre literales numéricos y producen un resultado numérico
        - `a+b`  Suma
        - `a-b`  Resta
        - `a*b`  Multiplicación
        - `a/b`  División
        - `a**b` Potencia
        - `a%b`  Módulo
    - **Relacionales**: se aplican principalmente sobre literales numéricos y siempre producen un resultado booleano
        - `a>b`  Mayor que
        - `a>=b` Mayor o igual que
        - `a<b`  Menor que
        - `a<=b` Menor o igual que
        - `a==b` Es igual a. También sobre `bool`
        - `a!=b` Es distinto a. También sobre `bool`
    - **Lógicos**: se aplican sobre expresiones booleanos y siempre producen un resultado booleano
        - `exp && exp` AND
        - `exp || exp` OR
        - `! exp` NOT
    - **Bitwise**: se aplican únicamente sobre `int` (interpretados como binarios) y producen un entero (interpretado como binario)
        - `&`   AND
        - `|`   OR
        - `^`   XOR
        - `<<`  shift left
        - `>>`  shift right
    - **Asignación**: se considera un syntax sugar para una asignación con los operadores matemáticos básicos: `+`, `-`, `*`, `/`, que quedarían como `+=`, `-=`, `*=`, `/=`.
    - **Flujo**: únicamente `if(exp_log, exp_true, exp_false)`, que sintácticamente es una llamada a función, pero semánticamente se comporta como un operador condicional (ternario), seleccionando entre dos expresiones en función de una condición booleana. Si `exp_log` es `true` el resultado es el resultado de `exp_true`, en caso contrario es el resultado de `exp_false`

#### Funciones

Las funciones son expresiones que representan una operación parametrizada reutilizable. A diferencia de los operadores, las funciones no forman parte de los operadores o construcciones sintácticas del lenguaje, sino que se evalúan mediante resolución en tiempo de ejecución.

Una función recibe una o más expresiones como argumentos y devuelve una única expresión como resultado.

Diferenciamos varios tipos de funciones. Por un lado están las definidas por el propio usuario durante el uso del intérprete. Son las funciones que más se ajustan a la definición que hemos dado. Por otro lado están las funciones externas cargadas en el intérprete. Aunque funcionen como una función al uso, son ejecutadas por el runtime del host y no por el propio intérprete. Dentro de estas podemos diferenciar entre las de gestión del entorno ([Entorno](#entorno)) y las cargadas dinámicamente ([Funciones externas](#funciones-externas))

### Statements

Definimos un statement como una estructura semántica que realiza alguna acción, pero no produce un valor de retorno. El ejemplo más claro de esto es una declaración o asignación.

Nótese que una expresión puede aparecer dentro de un statement, pero no al revés.

#### Declaración de variables

Las variables se declaran mediante un `id` y el símbolo `=` seguido de una expresión. Un ejemplo es `a = 3 + 2`, que declara la variable `a` con valor `5`, resultado de la expresión `3+2`. Si queremos dar un nuevo valor a `a` hacemos una nueva asignación.

Como se mencionaba en la sección [Operadores](#operadores), existen los syntax sugars `+=`, `-=`, `*=`, `/=`. Por ejemplo `a += exp` equivaldría a `a = a + exp`.

#### Declaración de funciones

Las funciones se declaran de la forma `name (args) = exp`, donde `name` es el nombre de la función, `args` son una serie de nombres de variables separados por comas, y `exp` una expresión del lenguaje.

Por ejemplo, si queremos definir la función que nos calcule la media harmónica de dos números podemos hacer `h(a, b) = (2 * a * b) / (a + b)`. Pero esto tiene un problema, y es la posible división por 0, que podemos gestionar haciendo `h(a, b) = if(a + b == 0, 0, (2 * a * b) / (a + b))`. Esto es así porque, como se comentó en [Operadores](#operadores). En este caso, si se cumple la condición `a + b == 0` devolvería 0, en caso contrario devolvería el resultado de la expresión `(2 * a * b) / (a + b)`. La función la llamaríamos de la forma `h(1, 2)`.

Nótese que al llamar a una función substituimos cada argumento por una expresión, por lo que podríamos hacer cosas como: `h(if(2>3, 3, 4), 2 ** 3)`. O también: `h(h(2,3), h(4,5))`.

O podríamos declarar una función recursiva: `fact(n) = if(n <= 1, 1, n * fact(n - 1))`.

##### Funciones externas

Las funciones externas como tal no son definidas por el usuario en ningún momento, sino que se vuelven accesibles a través de importar una librería externa. Una vez importada una librería nos permite llamar a sus funciones directamente, como si las hubiese definido el usuario. Para ver cómo importar consultar [Gestión del entorno](#gestión-del-entorno)

##### Sobre los `string`

Como se menciona en el apartado de [Expresiones](#expresiones) existen las expresiones que retornan un resultado de tipo `string`. Esto es interesante, porque los siguientes *statements* son válidos:

- `a = "hola"` ahora `a` es una variable que solo puede aparecer en una expresión `a`

- `f(a) = a` es un caso interesante, ya que es genérica a todos los tipos del lenguaje. `f(true)` retorna `true`; `f(1)` retorna `1`, así como `f("hola")` retorna `"hola"`.

- `f(a) = 2*a` emitiría un error al intentar evaluar `f("hola")`, pero también al intentar evaluar `f(true)`.

## Entorno

Para gestionar el entorno del intérprete existen disponibles una serie de comandos, que siguen la misma sintaxis que las funciones definidas en [Funciones](#funciones), pero precedidas de `:`. Esto es una estructura parecida a editores como `vim`, y permite diferenciar claramente los *comandos* para gestionar el propio intérprete de las *funciones* como expresiones del lenguaje.

### Gestión del entorno

- `:output("quiet" | "normal" | "echo" | "verbose")`
    - `"quiet"` Solo se imprimen errores por pantalla.
    - `"normal"` A mayores de lo anterior se imprime el resultado de las
                 expresiones.
    - `"echo"` A mayores de lo anterior, se imprime la sentencia evaluada.
    - `"verbose"` A mayores de lo anterior, se imprime el proceso de evaluación
                  para la obtención del resultado.
- `:warnings(bool)` A mayores del `output` seleccionado, imprime warnings.
- `:reset()` Borra las variables y funciones definidas por el usuario.
- `:restart()` Igual que `reset` pero descarga también librerías.
- `:env()` Lista las variables y funciones del entorno.
- `:libs()` Lista las librerías cargadas.
- `:load("fichero")` Lee una una ruta relativa en el sistema, carga el fichero y ejecuta todas las instrucciones que hay dentro, menos las de gestión del entorno. Esto es para evitar cargas recursivas, etc.

### Utilidades

- `:clear()` Limpia la pantalla.
- `:help([id])` Muestra ayuda general o de un identificador (como una función).
- `:history()` Muestra historial de la sesión.
- `:exit()` Cierra el intérprete.

## Librería Estándar

De forma estándar se cargan las utilidades matemáticas de `math.h` de C. Nótese que las funciones que se pueden realizar se reducen a las que comparten los tipos soportados por el lenguaje del intérprete.

### Constantes

Por defecto se definen las constantes matemáticas `PI`, `E`, `PHI`... Estas no se pueden modificar, ni se pueden agregar más.

## Gestión de errores

El intérprete distingue dos niveles de incidencias durante el procesamiento del programa: **errores** y **warnings**. Ambos pueden producirse en distintas fases del intérprete: análisis léxico/sintáctico, análisis semántico o ejecución.

### Errores

Un error es una condición que impide la correcta finalización de la evaluación de una expresión o *statement*. Cuando ocurre un error, la ejecución de la instrucción afectada se interrumpe y no se produce ningún valor de retorno.

Los errores pueden clasificarse según la fase en la que se producen:

- **Errores léxicos/sintácticos**: entrada no reconocida por la gramática del lenguaje.

- **Errores semánticos**: violaciones de las reglas del lenguaje, como:
    - uso de operadores no definidos para los tipos dados (`1 + "hola"`)
    - incompatibilidad de tipos en expresiones (uso de bool en operaciones no lógicas)
    - uso de identificadores no definidos

- **Errores en tiempo de ejecución**: fallos durante la evaluación de expresiones o funciones (división por cero)

### Warnings

Un warning es una condición que no impide la ejecución de la expresión o statement, pero indica que el intérprete ha realizado una conversión o inferencia implícita que puede alterar el resultado esperado.

Los warnings se generan, por ejemplo, en los siguientes casos:

- conversión implícita de `int` a `double`

Los warnings no interrumpen la ejecución, pero pueden ser activados o desactivados mediante la configuración del entorno.

## Por definir

- tipado fuerte de variables: si se define como `int` no asignar otro dato, o castear `double` a int -> warning coercion tipos y perdida precision
- operadores bitwise -> coercion double -> int?
