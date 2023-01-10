# P1_T1_PARALELA
Ejemplo de sentencia de compilación:
mpicc -lm -o [nombre-ejecutable][nombre_fichero_fuente]
mpicc -lm -o main main.c
-lm lo estamos usando porque necesitamos linkar la librería de math.h
Ejemplo de sentencia de ejecución:
mpirun ./[ nombre-ejecutable][fichero_img_.raw][ancho][largo][media, mediana o sobel]
mpirun ./main lena4096x4096.raw 4096 4096 media
