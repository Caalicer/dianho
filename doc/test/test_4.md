# Cuestionario Práctica 3

**¿Cuáles son los distintos tipos de símbolos que necesitas guardar en la tabla de símbolos? ¿Qué información necesitas guardar para cada uno de ellos? ¿Qué estructura de datos utilizas para almacenar cada uno de ellos?**

Necesitamos almacenar los operadores matemáticos, así como operaciones matemáticas específicas. Necesitamos también almacenar variables que se asignen en tiempo de compilación, constantes matemáticas y las funciones propias del intérprete (como help, exit...). Dependiendo del tipo de símbolo usado se requerirá almacenar diferente información (por ejemplo, para las variables de usuario y las constantes matemáticas es necesario almacenar el valor actual que tienen, pero para las funciones será interesante almacenar también el tipo de datos que devuelven, información de los argumentos, etc).
Se puede usar %union para almacenarlos, así como structs.

---

**¿Cuáles son los distintos componentes léxicos reconocidos durante la fase de análisis léxico?**

Operadores matemáticos, operadores de asignación, funciones de gestión interna (exit, load, help...), variables matemáticas.

---

**¿Cómo gestionas de manera diferenciada la aparición de un símbolo en el lado derecho o en el lado izquierdo de una sentencia de asignación?**

Debemos tener en cuenta que no todas las posibilidades de asignación son posibles. Debemos gestionar mediante errores (que no implican el cierre del propio intérprete, sino un aviso de fallo en la sentencia) los distintos casos posibles.

Deben avisarse de fallo en caso de intentar realizar una asignación a una función matemática (como coseno), a una función del propio intérprete (como EXIT), a una constante matemática (como pi o e), o a funciones que se hayan cargado dinámicamente.

En el lado derecho de la asignación, debe gestionarse el uso de variables no definidas previamente (como `b=a+1` si `a` no se ha definido).

---

**¿Cómo podrías añadir nuevas funciones a la calculadora sin necesitar recompilar el fichero de descripción del léxico o de la gramática?**

Podríamos aplicar un modelo de gestión dinámica de librerías a través de `dlfcnc.h`. Esto nos permitiría no cargar toda la librería en memoria, y realizar en tiempo de compilación la búsqueda de estas librerías dinámicas. Una vez encontramos una función, se almacena en la tabla de símbolos, para posteriores llamadas. Para esto se hace uso del a funcion dfsym(), para buscar dicha función.

Otra aproximación (que se debe evitar) es cargar toda la libreria directamente en la tabla de símbolos, ya que no se puede saber el número total de símbolos a cargar, y puede hacerse trabajo excesivo.
