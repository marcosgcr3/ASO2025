/*escribe un programa que • Obtenga la fecha actual usando time() y la muestre en la salida estándar usando un formato de fecha legible
utilizando la función auxiliar ctime().
• Calcule la fecha de hace diez días y la muestre en la salida estándar, también en formato legible usando ctime().
Responder a las siguientes cuestiones:
• ¿Dónde se reserva espacio para el valor de la cadena que devuelve la función ctime()?
• ¿Es necesario liberar el puntero?*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    time_t t = time(NULL);
    if (t == -1) {
        perror("Error en time");
        return 1;
    }
    printf("Fecha actual: %s", ctime(&t));
    t -= 10 * 24 * 60 * 60;
    printf("Fecha de hace diez días: %s", ctime(&t));
    return 0;
}
/*¿Dónde se reserva espacio para el valor de la cadena que devuelve la función ctime()?
El espacio para el valor de la cadena que devuelve la función ctime() se reserva en una zona de memoria estática que es compartida por todas las llamadas a ctime(). Por tanto, el valor devuelto por ctime() no debe ser modificado.
• ¿Es necesario liberar el puntero?
No, no es necesario liberar el puntero.*/
