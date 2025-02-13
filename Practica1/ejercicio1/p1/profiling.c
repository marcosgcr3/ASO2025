/*Cuando es necesario obtener la información horaria con precisión de microsegundos se puede usar gettimeofday().
Escribir un programa que mida cuánto tarda un bucle de 10000 iteraciones. En cada iteración, se
incrementará una variable de tipo int llamada mivar con un valor entero que se debe pasar al programa como
argumento. Si no se pasa este argumento el programa mostrará un mensaje de error y terminará devolviendo el valor 1.
En otro caso el programa mostrará por la salida estándar el valor final de mivar y el tiempo transcurrido, retornando el
programa el valor 0.*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <valor>\n", argv[0]);
        return 1;
    }
    int mivar = atoi(argv[1]);
    struct timeval t0, t1;
    if (gettimeofday(&t0, NULL) == -1) {
        perror("Error en gettimeofday");
        return 1;
    }
    for (int i = 0; i < 10000; i++) {
        mivar++;
    }
    if (gettimeofday(&t1, NULL) == -1) {
        perror("Error en gettimeofday");
        return 1;
    }
    printf("Valor final de mivar: %d\n", mivar);
    printf("Tiempo transcurrido: %ld microsegundos\n", (t1.tv_sec - t0.tv_sec) * 1000000 + t1.tv_usec - t0.tv_usec);
    return 0;

}
