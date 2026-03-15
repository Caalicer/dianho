# Test sesión 2

- Supongamos la hipotética aparición en la entrada de la cadena 0b0x3. ¿Debería el analizador léxico detectar e informar de algún error? ¿Qué secuencia de componentes léxicos se devolvería al analizador sintáctico?

Depende cómo queramos gestionar estos fallos. Podría devolverse un fallo al detectarlo, siendo un error léxico, y no enviándose ningún componente léxico al analizador sintáctico. Podríamos, por otra parte, interpretarlo como un fallo sintáctico, por lo que se devolvería al analizador léxico el número binario "0b0" y el identificador "x3", y sería éste el que lanzaría un error sintáctico, ya que no podrían estar contiguos un número binario y un identifcador : "x3".

En compiladores modernos no es común que se emitan fallos léxicos. Es preferible emitirlos más tarde, aunque se pueda perder precisión en la detección del fallo, pero se simplifica el analizador léxico, y el analizador sintáctico ya se especializa en la detección de errores.

---

- ¿Cuál es la secuencia de acciones necesaria para gestionar adecuadamente la tabla de símbolos ante la aparición de una cadena alfanumérica? Describe las funciones que deberán ejecutarse y la salida esperada en cada una de ellas?

Teóricamente, deberíamos analizar mediante un autómata la cadena alfanumérica, para detectar cuando empieza, termina y si está bien formada. A la hora de implementarlo en nuestro código, deberemos encapsular este análisis en una estructura condicional (un switch por ejemplo), que mediante una variable de estado transicione a diferentes casos, con su respectiva función de evaluación.

Una vez reconocida nuestra cadena alfanumérica deberemos insertarla en tabla de símbolos mediante una función de insertar. En este punto es importante haber precargado en la fase de inicio del compilador (antes de analizar ningún código fuente) los keywords. Esto es así porque existe ambiguendad entre los mismos y los identificadores. De esta forma, al estar precargados, se puede diferenciar si la cadena detectada es un keyword (ya presente en la tabla de símbolos), o un identificador. La tabla de símbolos debe devolver el componente léxico si ya estaba insertado en la tabla, y sino insertarlo y también devolver el componente léxico asociado.

---

- ¿Qué estructura de datos empleas para implementar la tabla de símbolos? ¿Cómo explotas el orden a la hora de realizar búsquedas e inserciones?

Existen múltiples opciones, y todas válidas. Considero, que si se implementa de forma debida, una tabla hash podría ser una forma muy eficiente. Si la función de hash es buena (no produce muchas colisiones), y el tamaño de la tabla es adecuado (y dinámico), la complejidad computacional de inserción O(log(n)) si usamos un algoritmo de búsqueda binario (es una estructura ordenada), y constante en la inserción.

En mi caso usaré un AVL, ya que es totalmente dinámico, y tiene un método de balanceo dinámico.

Por otro lado, si queremos hacer una versión más sofisticada de nuestro analizador, teniendo en cuenta los diferentes bloques del código, podría ser interesante otra estructura de datos, como la de bosque, vista en clase, con árboles AVL por cada nivel, pudiendo explotar el orden a la hora de buscar mediante algoritmos de búsqueda binaria (O(log(n))).

---

- ¿Cómo construyes un lexema a partir de la secuencia de caracteres leídos de la entrada?

Mediante una estructura condicional que implemente la funcionalidad de un autómata, transicionando entre casos según se cambie de estado y se lean las entradas. Podemos emplear un switch, con funciones específicas para cada tipo de autómata, obteniendo más encapsulación y sostenibilidad del código.

---

- ¿Cómo haces disponible el lexema actual para el analizador sintáctico?

Se trata del problema productor consumidor. Se insertaría en una estructura de datos común a ambos, donde el productor (analizador léxico) introduce productos (lexemas) que el consumidor (analizador sintáctico) consume mediante una función, como podría ser `siguienteComponenteLexico()`.
