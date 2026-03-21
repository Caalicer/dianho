# Analizador léxico de Dlang

Construir un analizador léxico que devuelva los componentes léxicos que aparecen en el programa `regression.d`, codificado en el lenguaje D.

## SPECS de mi sistema

- `OS`: *Ubuntu 22.04.5 LTS x86_64*
- `gcc`: *gcc (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0*
- `make`: *GNU Make 4.3*
- `valgrind`: *valgrind-3.18.1*
- `gdb`: *GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1*

## Estructura del proyecto

- proyecto/ (carpeta raíz)
    - bin/ (ejecutable final)
    - src/ (código fuente, cada .c)
    - inc/ (cabeceras, cada .h)
    - obj/ (archivos .o)
    - LICENSE (licencia de uso)
    - Makefile
    - README.md
    - regression.d

### Docuemntación
Todas las funciones están documentadas en el `.h` asociado siguiendo el estilo de `doxygen`. Algunas tienen notas más extensas sobre cuestiones de diseño.

En algunos ficheros `.h` como `dfa.h` tienen al inicio de todo notas explicando patrones de diseño usados, y que se repiten en otras partes del código. Un ejemplo principal es el uso de `enums` como índices en arrays que se usan como mappers.

## Uso del Makefile

Si ejecutas `make` sin ninguna regla se imprimirá por defecto la regla `help`

```sh
make help
Opciones disponibles:
	 release    - Versión final sin archivos intermedios
	 debug      - Versión de depuración (con archivos intermedios)
	 runrelease - Ejecuta la versión final
	 rundebug   - Ejecuta la versión de depuración
	 valgrind   - Ejecuta con valgrind la versión de depuración
	 gdb        - Ejecuta con gdb la versión de depuración
	 clean      - Limpieza de archivos generados
	 cleanall   - Limpieza completa de directoriso generados
	 help       - Guia de uso del Makefile
```

Las reglas principales para generar un ejecutable son `release` y `debug`. Ambas generan un ejecutable llamado `lexer` que se situará en `./bin/`. La diferencia principal entre ambas es que `debug` activa flags del compilador, como warnings extendidos, o variables de debugueado.

Las reglas del tipo `run` ejecutan el binario generado por su respectiva regla previa. No es necesario especificar el fichero de entrada, porque en el propio Makefile toma por defecto `regression.d` situado en la raiz del proyecto.

Las opciones de `valgrind` y `gdb` lanzan respectivamente el binario de depuración a través de estas herramientas, sin parámetros.

Las reglas de limpieza es recomendable usarlas si modificas alguno de los ficheros `.h` que no tienen un `.c` asociado. Si no haces esto puedes estar ejecutando versiones antiguas del codigo.

## Probar el código

1. Modificar el tamaño máximo de lexema -> en `definiciones.h` y hacer un `cleanall` primero.
2. Modificar los autómatas del lexer -> en `automatas.h` y `cleanall` primero.

## Sobre el diseño

### Enums y mappers

Explicado en `dfa.h`. Consiste en crear un `enum` con elementos como `{Lunes, Martes, Miercoles, ..., COUNT}` que nos sirvan como índices.

Luego podríamos crear un array de cualquier tipo, llamado `mapa[COUNT]` que tiene de tamaño el número de elementos reales del `enum`. Esto es así porque los `enum` comienzan en 0.

Luego podemos usar ese `enum` como índice para acceder a nuestro array: `mapa[Lunes]` accedería al elemento asociado al Lunes. Es interesante si tenemos, por ejemplo, una lista de autómatas, y queremos poder escoger uno con una función que calcule el automata y devuelva un entero (el `enum`). Con este concepto podemos modularizar mucho el código.

### Punteros a funciones

Se usa en múltiples sitios, como en el AVL para pasar funciones propias que queramos ejecutar en cada nodo (como la de imprimir); o en `lexico.c` para modularizar la gestión de un autómata. La sintaxis de los punteros a funciones puede ser *bastante* confura.

### Analizador léxico

Aquí se optó por hacerlo totalmente modular e independiente del lenguaje que queramos analizar. Para ello definidos un tipo `dfa` que generaliza autómatas finitos. Para modificar dichos autómatas hay que ver el fichero `automatas.h`

### Copias de un mismo lexema

Para evitar copiar un mismo lexema tontamente se crea el tipo `fragments`. Se trata de una estructura que contiene un array segmentado en dos fragmentos, y este viene definido por un puntero a cada fragmento, y el tamaño del mismo.

Con esta estructura podemos devolver el fragmento que apunte a los datos en el propio sistema de entrada, y habilitar en la tabla de símbolos (y respectivamente en el AVL) la búsqueda por fragmentos. Para realizarlo se usan tanto punteros a funciones (AVL), como nuestra propia función de comparación de strings segmentados: `_fragcmp_impl`, definida en `tabla_simbolos.h`.

### Gestión de errores

Se implementan `warnings` que están desactivados por defecto. Estos son avisos que *pueden* ser un posible fallo, pero no está garantizado.

Por ejemplo el caso de `std.excection`. En la parte de `.` se entra en el autómata de números, y como `e` es un símbolo del alfabeto de dicho autómata, podría ser que se estuviese queriendo hacer un número exponencial y no fuese correcto: `1.e10`.

En general estos warnings se asocian a cuando no hay transiciones definidas para un estado con alguno de los símbolos del alfabeto.
