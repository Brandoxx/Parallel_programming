## Goldbach - Implementacion Distribuida

## Descripcion


Las sumas de Goldbach son sumas compuestas por numeros primos, se hipotetiza que cualquier numero mayor a 5 puede ser formado por una suma de Goldbach dada por:


1. Dos sumas, si el numero es par.
2. Una suma, si el numero es impar.


Este programa encuentra las sumas de Goldbach de diversos numeros a partir de un input del usuario. Esta implementacion es concurrente y distribuida.


##Requerimentos 


1. **Sistema operativo Linux o base UNIX con estandar POSIX** 


## Installation


El projecto posee un MakeFile para construirlo.


En la carpeta del programa abre una terminal, dentro corre `make` en una linea de comandos.



## Usage

El programa admite un argumento por linea de comando; este argumento será el numero de hilos que el programa podrá utilizar. Si no se provee, se asumirá el numero máximo de 
núcleos reportados al sistema operativo.


Al correr el programa, se deben ingresar uno por uno los numeros a calcular. Para finalizar se debe enviar un EOF (End Of File), representado por ctrl + d.


Si un numero positivo valido es ingresado, el programa imprimirá una lista con los números en el el mismo orden en que fueron ingresados. Cada línea de la lista contiene un número ingresado seguido por la cantidad de sumas de Goldbach que tiene.


Si un numero negativo valido es ingresado, ademas mostrara cada una de las sumas correspondientes al numero, separadas por comas.


Si se ingresa un numero menor o igual a 5 (mayor o igual a -5, para el caso de negativos), o un valor fuera del rango permitido, el programa muestra NA como resultado.


Si se ingresa algo que no es un numero, el programa lo cataloga como invalido y cesa su ejecucion.


Si se ejecuta el programa sin mpiexec, el programa se ejecutara en modo concurrente, con la cantidad de hilos especificados por el usuario.


Si se ejecuta el programa con mpiexec, el programa se ejecutara en modo distribuido, con la cantidad de hilos especificados por el usuario. Cada proceso se encargará de calcular las sumas de Goldbach de un numero, y el proceso 0 se encargará de imprimir los resultados.

##Ejemplos de ejecucion


###Numero validos:


####Input:


    bin/goldbach_serial
    1422
    7498
    6327
    5718
    7591
    6652
    3901
    5661
    6236
    7502
    3375
    4895
    6417
    8740
    8458
    7845
    3819
    4504
    6062
    9716
    7465
    4026
    1523
    5766
    1947
    8272
    9282
    3541
    5459
    2213

####Output: 


    Total 30 numbers, 172701 sums
    1422: 41 sums
    7498: 86 sums
    6327: 13601 sums
    5718: 127 sums
    7591: 26544 sums
    6652: 70 sums
    3901: 9035 sums
    5661: 11279 sums
    6236: 77 sums
    7502: 84 sums
    3375: 4645 sums
    4895: 12418 sums
    6417: 13953 sums
    8740: 136 sums
    8458: 84 sums
    7845: 17732 sums
    3819: 6019 sums
    4504: 58 sums
    6062: 76 sums
    9716: 117 sums
    7465: 23984 sums
    4026: 114 sums
    1523: 2192 sums
    5766: 136 sums
    1947: 2127 sums
    8272: 101 sums
    9282: 254 sums
    3541: 7736 sums
    5459: 16002 sums
    2213: 3873 sums


###Numeros fuera de rango:


####Input: 


    bin/goldbach_serial
    -5
    -4
    -3
    -2
    -1
    0
    1
    2
    3
    4
    5


####Output: 


    Total 11 numbers, 0 sums
    -5: NA
    -4: NA
    -3: NA
    -2: NA
    -1: NA
    0: NA
    1: NA
    2: NA
    3: NA
    4: NA
    5: NA


###Numeros negativos:


####Input: 


    bin/goldbach_serial
    -11
    -12
    -13
    -14
    -15
    -16
    -17
    -18
    -19
    -20
    -21
    -22
    -23
    -24
    -25
    -26
    -27
    -28
    -29
    -30
    -31


####Output: 


    Total 21 numbers, 72 sums
    -11: 2 sums: 2 + 2 + 7, 3 + 3 + 5
    -12: 1 sums: 5 + 7
    -13: 2 sums: 3 + 3 + 7, 3 + 5 + 5
    -14: 2 sums: 3 + 11, 7 + 7
    -15: 3 sums: 2 + 2 + 11, 3 + 5 + 7, 5 + 5 + 5
    -16: 2 sums: 3 + 13, 5 + 11
    -17: 4 sums: 2 + 2 + 13, 3 + 3 + 11, 3 + 7 + 7, 5 + 5 + 7
    -18: 2 sums: 5 + 13, 7 + 11
    -19: 3 sums: 3 + 3 + 13, 3 + 5 + 11, 5 + 7 + 7
    -20: 2 sums: 3 + 17, 7 + 13
    -21: 5 sums: 2 + 2 + 17, 3 + 5 + 13, 3 + 7 + 11, 5 + 5 + 11, 7 + 7 + 7
    -22: 3 sums: 3 + 19, 5 + 17, 11 + 11
    -23: 5 sums: 2 + 2 + 19, 3 + 3 + 17, 3 + 7 + 13, 5 + 5 + 13, 5 + 7 + 11
    -24: 3 sums: 5 + 19, 7 + 17, 11 + 13
    -25: 5 sums: 3 + 3 + 19, 3 + 5 + 17, 3 + 11 + 11, 5 + 7 + 13, 7 + 7 + 11
    -26: 3 sums: 3 + 23, 7 + 19, 13 + 13
    -27: 7 sums: 2 + 2 + 23, 3 + 5 + 19, 3 + 7 + 17, 3 + 11 + 13, 5 + 5 + 17, 5 + 11 + 11, 7 + 7 + 13
    -28: 2 sums: 5 + 23, 11 + 17
    -29: 7 sums: 3 + 3 + 23, 3 + 7 + 19, 3 + 13 + 13, 5 + 5 + 19, 5 + 7 + 17, 5 + 11 + 13, 7 + 11 + 11
    -30: 3 sums: 7 + 23, 11 + 19, 13 + 17
    -31: 6 sums: 3 + 5 + 23, 3 + 11 + 17, 5 + 7 + 19, 5 + 13 + 13, 7 + 7 + 17, 7 + 11 + 13

## Authors and acknowledgment


Brandon Alonso Mora Umana
brandon.moraumana@ucr.ac.cr
