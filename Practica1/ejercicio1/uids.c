/*Escribir un programa uids.c que muestre:
• El uid efectivo y el uid real del usuario que ejecutó el programa, e indique si el correspondiente ejecutable tiene
activado el bit setuid.
• Adicionalmente, utilizando la llamada getpwuid() mostrar el nombre de usuario del usuario real, su directorio
home y el shell que utiliza*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
int main() {
    printf("UID efectivo: %d\n", geteuid());
    printf("UID real: %d\n", getuid());
    if (geteuid() != getuid()) {
        printf("El bit setuid esta activado\n");
    } else {
        printf("El bit setuid no esta activado\n");
    }
    


    struct passwd *pw = getpwuid(getuid());
    if (pw == NULL) {
        perror("Error en getpwuid");
        return 1;
    }
    printf("Nombre de usuario: %s\n", pw->pw_name);
    printf("Directorio home: %s\n", pw->pw_dir);
    printf("Shell: %s\n", pw->pw_shell);
    
    return 0;
}