# concurrente22b-Brandon_moraumana

Curso de programación paralela y concurrente, contiene ejemplos, tareas y otros materiales varios.


# Glosario

* Programación serial: Programación de un solo hilo de ejecución. Sigue el paradigma de programación secuencial.

* Programación concurrente: Programación de varios hilos de ejecución. Sigue el paradigma de programación concurrente.

* Programación paralela: Programación de varios hilos de ejecución. Sigue el paradigma de programación paralela.

* Concurrencia de tareas: Concurrencia de tareas es la capacidad de un sistema de ejecutar varias tareas al mismo tiempo, en un contexto de multitarea.

* Paralelismo de datos: Paralelismo de datos se refire a la capacidad de varios hilos de ejecución de acceder a los mismos datos al mismo tiempo.

* Hilo de ejecución: Hilo de ejecución es un flujo de control de un programa. Los provee el sistema operativo.

* Indeterminismo: Indeterminismo es la propiedad de un sistema que no puede predecir el resultado de una ejecución. Se debe a que el sistema puede tener varios estados posibles.

* Memoria privada y compartida: Memoria privada es la memoria que cada hilo de ejecución tiene asignada. Memoria compartida es la memoria que todos los hilos de ejecución pueden acceder. La memoria compartida es más insecure que la memoria privada.

* Espera activa: Espera activa se refiere a la espera de un evento que no se sabe cuándo va a ocurrir. El hilo se queda esperando en un bucle infinito, y consume recursos del sistema.

* Condición de carrera: Condición de carrera se refiere a la situación en la que dos o más hilos de ejecución acceden a la misma memoria compartida y uno de ellos modifica el valor de la memoria compartida. El otro hilo de ejecución puede leer el valor modificado, y el resultado puede ser indeterminado.

* Control de concurrencia: Control de concurrencia es la capacidad de un sistema de controlar la concurrencia de tareas. Se puede hacer con semáforos, monitores, etc. El control de concurrencia es necesario para evitar condiciones de carrera.

* Seguridad condicionalÑ Seguridad condicional se refiere a utilizar mecanismos seguros, como arrays, para evitar condiciones de carrera.

* Exclusión mutua: Exclusión mutua se refiere a la capacidad de un sistema de asegurar que sólo un hilo de ejecución puede acceder a una sección crítica a la vez.

* Semáforo: Semáforo es un mecanismo de control de concurrencia que permite a un hilo de ejecución acceder a una sección crítica, y bloquea a los demás hilos de ejecución. 

* Barrera: Barrera es un mecanismo de control de concurrencia que permite a un hilo de ejecución esperar a que todos los demás hilos de ejecución lleguen a la barrera, y luego permite a todos los hilos de ejecución continuar.

* Variable de condición: Variable de condición es un mecanismo de control de concurrencia que permite a un hilo de ejecución esperar a que se cumpla una condición, y luego permite a todos los hilos de ejecución continuar.

* Candado de lectura y escritura: El candado de lectura y escritura es un mecanismo de control de concurrencia que permite a varios hilos de ejecución leer una variable compartida, pero sólo permite a un hilo de ejecución escribir en la variable compartida.

* Descomposición: Descomposición es la capacidad de un sistema de dividir un problema en subproblemas más pequeños, y resolverlos de forma concurrente. 

* Mapeo: Mapeo es la capacidad de un sistema de asignar tareas a hilos de ejecución. El mapeo puede ser estático o dinámico. 

* Incremento de velocidad: Incremento de velocidad es la capacidad de un sistema de incrementar la velocidad de ejecución de un programa. El incremento de velocidad se puede lograr con programación paralela, programación concurrente, o ambas.

* Comunicación punto a punto entre procesos: Comunicación punto a punto entre procesos es la capacidad de un sistema de enviar mensajes entre procesos. Los mensajes pueden ser síncronos o asíncronos.

* Comunicación colectiva entre procesos: Sistemas de comunicacion permite enviar mensajes a todos los procesos. 

* Reducción: Reducción es la capacidad de un sistema de aplicar una operación a todos los elementos de un arreglo, y devolver un único valor. Tambien se puede aplicar a valores enviados por mensajes.