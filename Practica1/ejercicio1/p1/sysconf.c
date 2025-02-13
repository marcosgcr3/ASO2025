/*Las funciones sysconf() y pathconf() permiten consultar información sobre la configuración del sistema y el
sistema de ficheros respectivamente. Escribir un programa que muestre los siguientes parámetros y reportar los valores
obtenidos en los puestos del laboratorio.*/
#include <stdio.h>
#include <unistd.h>
int main() {
    printf("Ticks por segundo: %ld\n", sysconf(_SC_CLK_TCK));
    printf("Numero maximo de procesos simultaneos por usuario: %ld\n", sysconf(_SC_CHILD_MAX));
    printf("Numero maximo de ficheros que puede abrir un proceso: %ld\n", sysconf(_SC_OPEN_MAX));
    printf("tamaño de las paginas de memoria: %ld\n", sysconf(_SC_PAGESIZE));
    printf("La longitud maxima de los argumentos de un programa: %ld\n", sysconf(_SC_ARG_MAX));
    printf("Numero maximo de enlaces a un fichero en el sistema de ficheros: %ld\n", sysconf(_PC_LINK_MAX));
    printf("Tamaño maximo de una ruta en el sistema de ficheros: %ld\n", sysconf(_PC_PATH_MAX));
    printf("El tamaño maximo de un nombre de fichero: %ld\n", sysconf(_PC_NAME_MAX));

    
    
    return 0;
}
