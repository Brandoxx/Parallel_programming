## Enunciado 

Modifique su solución de Ejemplo 7 para inicializar todos los semáforos en 0. Luego haga que el hilo principal incremente el semáforo del hilo 0. ¿Produce el mismo resultado que su solución anterior?

R/ Si, produce el mismo resultado.

¿Qué pasa si el hilo principal incrementa el semáforo del hilo 0 después de hacer su saludo Hello from main thread?

R/ El hilo principal siempre imprime primero, sin importar cuantos threads existan.