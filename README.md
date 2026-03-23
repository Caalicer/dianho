# Analizador léxico de Dlang

Construir un analizador léxico que devuelva los componentes léxicos que aparecen en el programa `regression.d`, codificado en el lenguaje D, usando la herramienta *Flex*

## SPECS de mi sistema

- `OS`: *Ubuntu 22.04.5 LTS x86_64*
- `gcc`: *gcc (Ubuntu 11.4.0-1ubuntu1~22.04.3) 11.4.0*
- `make`: *GNU Make 4.3*
- `valgrind`: *valgrind-3.18.1*
- `gdb`: *GNU gdb (Ubuntu 12.1-0ubuntu1~22.04.2) 12.1*
- `flex`: *flex 2.6.4*

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

> [!NOTE] Archivos de flex
>
> En este caso la regla `cleanall` elimina también los archivos generados por
> Flex: `lex.yy.c`
