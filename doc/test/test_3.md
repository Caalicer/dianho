# Test sesión 3

- En la arquitectura de compilación que has desarrollado, ¿existe algún límite máximo en el tamaño de un lexema? ¿A qué se debe este límite?

En la estructura desarrollada para la carga de fichero hacemos uso de un doble buffer con centinela. Esta estructura de datos permite que gestionemos ficheros de cualquier tamaño, sin límite. Por el contrario, el tamaño máximo de lexema se corresponde con el tamaño del bloque (de uno de ellos, sin tener en cuenta la posición reservada como EOF). Existe esta limitación, pues podría darse el caso de que para identificar un lexema se necesite cargar 3 bloques, sobreescribiendo el principio del lexema antes de terminar de identificarlo.

---

- ¿En qué casos es necesaria la devolución de caracteres al sistema de entrada?

Es necesaria cuando en el analizador léxico, en el autómata que está consumiendo los caracteres de entrada, llega a un estado de aceptación y necesita leer un caracter más para identificar la cadena. En este punto, acepta la cadena, y devuelve el carácter al sistema de entrada. Se trata en los casos de identificación de cadenas alfanuméricas, por ejemplo.

---

- ¿Qué ocurre si durante una operación de devolución de caracteres es necesario efectuar un cambio de bloque en el buffer del sistema de entrada?

Hay que gestionar bien la función de "devolver" del sistema de entrada. Debemos tener en cuenta que el último carácter del componente léxico puede alinearse con el fin del buffer. En este caso se cargaría el siguiente, se movería el puntero al nuevo bloque cargado. Aquí el analizador léxico llamaría a "devolver". Si volviera al bloque anterior, volvería a leer el fin de bloque, se volvería a cargar el siguiente bloque, machacando el existente que aún no hemos analizado.

---

- En el caso de un identificador, ¿cuántas copias realizas de cada uno de los caracteres que lo conforman desde el fichero fuente hasta su devolución al analizador sintáctico?

Necesitamos dos copias en total. Una es la que tenemos cargada en los buffers del sistema de entrada. La segunda es la almacenada en la tabla de símbolos. Para cualquier otro caso, podríamos usar punteros a estas copias según en la fase del proceso en la que estuviéramos. Por ejemplo, para imprimirlo por pantalla al final usaríamos un puntero a la copia almacenada en la tabla de símbolos. Podría existir una tercera copia si se vuelve a leer el mismo lexema posteriormente, y es necesario comprobar que ya está cargado en la tabla de símbolos.

---

¿Qué tipos de errores eres capaz de detectar y gestionar durante la fase de análisis léxico?

Debemos gestionar los errores tanto del sistema de entrada, así como errores léxicos. Por ejemplo, podemos interpretar como error léxico que un número esté más escrito. Todo dependerá de cuanto queramos que abarque la gestión de ese tipo de errores el analizador léxico.
