/*El comando del sistema uname muestra información sobre diversos aspectos del sistema. Escribir un programa que
muestre por la salida estándar los detalles del sistema que reporta uname y que sea equivalente a ejecutar el comando: uname -s -n -r -v -m*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>


int main() {
    struct utsname buf;
    if (uname(&buf) == -1) {
        perror("Error en uname");
        return 1;
    }
    printf("Nombre del sistema: %s\n", buf.sysname);
    printf("Nombre del nodo: %s\n", buf.nodename);
    printf("Release del sistema: %s\n", buf.release);
    printf("Versión del sistema: %s\n", buf.version);
    printf("Hardware: %s\n", buf.machine);
    return 0;
}


