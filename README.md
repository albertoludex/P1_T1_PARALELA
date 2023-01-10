# P1_T1_PARALELA
El objetivo de esta práctica es programar con el paradigma MPI para sistemas de memoria distribuida. Se leerá una imagen en tonos de grises almacenada en fichero binario con extensión raw, es decir los datos serán de tipo unsigned char y el tamaño del fichero será de altura x anchura x 1byte.  
La imagen leída se almacenará en una matriz bidimensional por el proceso root, las imágenes procesadas se almacenarán en ficheros binarios en formato raw, proceso realizado por el proceso root.  Procesados a implementar (se escogerá uno pasando parámetro):  
Filtrado por media (3 x 3 elementos)  
Filtrado por mediana (3 x 3 elementos)  
Detección de bordes (SOBEL)               
      C                                             F
‐1   0   1                                    ‐1  ‐2  ‐1
‐2   0   2                                     0   0   0
‐1   0   1                                     1   2   1 
                  Fij = sqrt (C2 + F2) 

El fichero texto de salida, generado por el proceso root contendrá la siguiente información: nombre de fichero de entrada, tamaño de la imagen, nombre de fichero de salida, número de procesos utilizado, tiempo de ejecución (contado desde que el root ya tiene la imagen en memoria hasta antes de comenzar a gabar el fichero con la imagen procesada).  En la detección de bordes por SOBEL, aquellos elementos que no disponen de los 8 vecinos SI serán considerados, realizando extensión simétrica tanto para filas (la fila -1 será igual a la 1, la fila N será igual a la N-2, siendo N el número de filas) como para columnas.    
