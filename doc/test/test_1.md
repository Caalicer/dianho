# Test sesion 1

- ¿Qué funciones exportan los distintos módulos del analizador y, por tanto, se han de situar en cada fichero .h?

Las funciones que deben ser accesibles por otras partes del proceso de análisis. En el caso de del analizador léxico (lexico.c, lexico.h), debería tener en lexico.h una función siguienteComponenteLexico(). Es el analizador sintáctico el que se espera que llame a esta función para ir obteniendo los diferentes componentes léxicos de "regression.d". Se sigue un patrón Productor (analizador léxico) Consumidor (analizador sintáctico).

A su vez, se espera que en la tabla de símbolos (TS.c, TS.h) tenga definidas las funciones de buscar(), inicializar()... Que permita buscar elementos en la tabla, e inicializarla en el comienzo de todo el proceso (desde main.c). También deberían existir funciones para insertar, eliminar elementos...

Esto permite tener un código más claro y limpio, abstrayendo las diferentes partes del proceso. Se mantiene así un código escalable y de fácil mantenimiento a lo largo del tiempo.

- ¿Cuáles son los tipos de componentes léxicos presentes en el código fuente?

Los tipos de componentes léxicos, definidos en definiciones.h (mediante sentencias del tipo `#define IdComponente ValorNumerico") son los asociados al lenguaje D. Podemos consultarlos todos en la documentación del lenguaje (<https://dlang.org/spec/lex.html>).

El alcance de la práctica únicamente contemplará los presentes en "recurrsion.d". Se busca, entonces, un analizador léxico para una parte de D, no para el lenguaje en su totalidad. Eso no quiere decir que no se mantenga una visión generalista, y si solo aparecen los número del 1 al 5, nuestro analizador léxico no deba contemplar la totalidad de los número enteros (que debería).

En la clase identificamos "import" como una palabra reservada (keyword en la documentación de referencia), e identificadores en el código (nombre del paquete, de funciones)...

Existen otros ejemplos de componentes léxicos en el código, como números de diferentes tipos (hexadecimales, reales), delimitadores, comentarios, etc.
