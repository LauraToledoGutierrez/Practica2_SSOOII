# Practica2_SSOOII

Se solicita el diseño y codificación del buscador SSOOIIGLE, capaz de buscar información en un fichero de forma paralela con múltiples hilos.
En líneas generales, el usuario debe especificar al buscador el fichero donde buscar, la palabra que desea buscar y el número de hilos que se deben emplear en la búsqueda.

SSOOIIGLE <nombre_fichero> <palabra> <numero_hilos> 
  
De esta forma, el fichero se «dividirá1» en tantas partes como hilos se indiquen. Por ejemplo, imagine el caso en el que un fichero consta de 100 líneas. Si el usuario invocara al buscador de la siguiente forma:

SSOOIIGLE historia.txt castillo 2 
  
El proceso principal creará dos hilos, el primero de ellos buscará la palabra castillo entre las líneas 1 y 50 del fichero historia.txt, mientras que el segundo hilo realizará la misma búsqueda de forma paralela entre las líneas 51 y 100. 

Por otro lado, el buscador debe mostrar por pantalla la aparición de la palabra en el archivo con la siguiente información: 
  
    •Fragmento en el que se ha encontrado la palabra. A su vez un fragmento se define mediante un número de hilo, línea inicial y línea final.
  
    •Línea del fichero en la que ha sido encontrada. 
  
    •Referencia a las dos palabras entre las que se encuentra la buscada: palabra anterior y palabra posterior. 
  
Un requisito importante es que el resultado de búsqueda se debe mostrar en orden. Es decir, no se pueden mostrar los resultados de un fragmento posterior, aunque el hilo que haya realizado la búsqueda en ese fragmento haya finalizado antes. Un ejemplo de salida podría ser el siguiente: 

  [Hilo 1 inicio:0 – final: 25] :: línea 12 :: … el castillo antiguo … 
  
  [Hilo 1 inicio:0 – final: 25] :: línea 20 :: … un castillo abandonado …  
  
  [Hilo 3 inicio:51 – final: 75] :: línea 63 :: … el castillo y … 
  
  [Hilo 3 inicio:51 – final: 75] :: línea 72 :: … sin castillo y … 
  
  [Hilo 4 inicio:76 – final: 100] :: línea 92 :: … el castillo derrumbado …
  
El estudiante debe diseñar una estructura de datos donde almacenar las ocurrencias de una palabra de acuerdo al formato anterior. Esta estructura debe ser compartida por todos los hilos y se debe garantizar exclusión mutua en el acceso. Es decir, mientras un hilo acceda a la estructura para escribir una nueva ocurrencia, el resto debe esperar. 
