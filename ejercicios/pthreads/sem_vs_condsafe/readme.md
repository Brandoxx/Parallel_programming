## Enunciado 

Ejecute al menos tres veces los códigos de Ejemplo 7 (imprimir en orden con semáforos) y Ejemplo 8 (imprimir en orden con seguridad condicional) con tantos hilos como su sistema operativo permite. Reporte en su readme.md las tres duraciones de cada solución. Tome la menor duración de las tres corridas de cada versión. ¿Cuál solución realiza su trabajo en menor tiempo? Discuta en máximo un párrafo cuál podría ser la causa de este comportamiento.

## Resultado
Ejemplo 7: 2.146062960s
Ejemplo 8: 1.944632176s

La seguridad condicional es mas rapida. Esto se debe, a mi parecer, a que esta forma no limita el trabajo de cada thread.
Todos trabajan a maxima capacidad, y luego todo es impreso en orden. Con semaforos, cada thread tiene que esperar a su turno para trabajar.