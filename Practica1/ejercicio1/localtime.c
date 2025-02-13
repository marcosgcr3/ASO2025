#include <sys/types.h>
#include <time.h>
#include <stdio.h>

//consultar time(2), localtime(3), strftime(3)

int main()
{


/* Obtencion del tiempo actual (time_t) */
    time_t t = time(NULL);
    if (t == -1) {
        perror("Error en time");
        return 1;
    }
   
/* Obtencion de la estructura  (struct tm) */
   struct tm *tm = localtime(&t);
    if (tm == NULL) {
        perror("Error en localtime");
        return 1;
    }
    
/* Formatear la fecha en una cadena con el patron ejemplo
   "Hoy es Viernes 11:40" */
    char buf[100];
    if (strftime(buf, 100, "Hoy es %A %H:%M", tm) == 0) {
        perror("Error en strftime");
        return 1;
    }
    
/* Mostrar la cadena resultante */    
    printf("%s\n", buf);
  
    
    return 1;
}    
