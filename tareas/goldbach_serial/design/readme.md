#Design

##UML
![UML design for data structures](/design/UML_Diagram.png "UML design for data structures").


**Every arrow reprensents a relationship between every data structure**


Cada estructura de datos almacena dinamicamente el input, los numeros primos y las sumas de cada numero.


Array_number almacena el input del usuario, y crea dinamicamente un array de sumas para cada numero. Number_t almacena el numero, junto con varias flags importantes para saber que comportamiento debe tener el programa al tratar con el.


Array_sums es un array de arrays, recibe las sumas de un numero especifico y las almacena dinamicamente.


Array_prime_numbers es un array especialmente disenado para numero primos, almacena el numero primo y la cantidad de veces que ha aparecido en las sumas (importante para el calculo de numero impares)


##Memory Diagram


![Memory Diagram for data structures](/design/Memory_diagram.png
"Memory Diagram for data structures").


**Upper numbers represent an input from the user**


Este es un diagrama de memoria que representa el comportamiento de las estructuras de datos con varios inputs del usuario.


Es importante resaltar la importancia de las flags, y de como cada una va a determinar el comportamiento del programa al calcular las sumas.