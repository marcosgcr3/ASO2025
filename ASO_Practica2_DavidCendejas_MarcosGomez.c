#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>

#include <ctype.h>
#include <string.h>
#include <getopt.h>

// Opción -f: comprueba si es un fichero regular. 
	// Si el fichero no es regular sice que tipo de fichero es.
	// Si es regular muestra el nombre del fichero y entre paréntesis su número de inodo y su tamaño (en Kib si es mayor que 1024 bytes).

	// Opción -l (opción larga --lines):
		// Averigua si es un fichero regular de texto o de tipo binario. Si es de texto muestra también el número de líneas que contiene

void list_file(const char *filepath, int lines, FILE *output) {
    struct stat file_stat;
    if (lstat(filepath, &file_stat) == -1) {
        perror("Error obteniendo información del archivo");
        exit(EXIT_FAILURE);
    }
    
    if (!S_ISREG(file_stat.st_mode)) {
        fprintf(output, "%s no es un fichero regular, es de tipo: ", filepath);
 
        if (S_ISDIR(file_stat.st_mode)) { fprintf(output, "Directorio\n");} 
        else if (S_ISLNK(file_stat.st_mode)) { fprintf(output, "Enlace simbólico\n"); }
        else { fprintf(output, "Desconocido\n"); }
        
        return;
    }
    
    fprintf(output, "%s (inodo %lu, ", filepath, file_stat.st_ino);
    
    if (file_stat.st_size > 1024)
        fprintf(output, "%ld Kib)", file_stat.st_size / 1024);
    else
        fprintf(output, "%ld bytes)", file_stat.st_size);
    
    if (lines) {   //-l
        FILE *file = fopen(filepath, "r");
        if (!file) {
            perror("Error abriendo el archivo");
            return;
        }
        
        int nLineas = 0;
        int c;
        while ((c = fgetc(file)) != EOF) {
            if (c == '\n') nLineas++;
        }
        fclose(file);
        fprintf(output, ", %d lineas", nLineas);
    }
    
    fprintf(output, "\n");
}

// Opción -d: Comprueba si es un directorio sobre el que se tienen permisos de lectura y ejecución. 
	// Si se tienen dichos permisos, se mostrara un listado ordenado del contenido del directorio, indicando para cada entrada su tipo correspondiente

	// Opcion -l (opción larga --lines): 
		// solo se listarán los ficheros regulares de texto del directorio sobre los que se tenga permiso de lectura y se mostrará adicionalmente en el 		
		// listado el número de lineas que contienen cada uno.
		
	// Opcion -x (opción larga --exe): con esta opción solo se listarán los ficheros regulares que tengan permiso de ejecución.

	// PARTE OPCIONAL
	// Opcion -s (opción larga --symbolic): solo se listarán los enlaces simbólicos. 
		// El fichero al que apuntan estos enlaces se mostrará así (enlace -> /ruta/destino). 
		// Si la ruta destino no existe o no se tiene acceso a la misma se añadirán 3 símbolos de interrogación (por ejemplo: enlace -> /destinoroto???).
		
	// Opcion -r (opción larga --recursive): con esta opción se listarán de forma recursiva los subdirectorios contenidos
		//en el directorio. Como argumento opcional se puede indicar la profundidad máxima.

void list_directory(const char *dirpath, int lines, int exe, int symbolic, int recursive, int profundidad, FILE *output) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        perror("Error abriendo el directorio");
        exit(EXIT_FAILURE);
    }
    
    struct dirent *entry;
    struct stat file_stat;
    char ruta[1024];
    
    while ((entry = readdir(dir)) != NULL) {
        snprintf(ruta, sizeof(ruta), "%s/%s", dirpath, entry->d_name);
        
        if (lstat(ruta, &file_stat) == -1) continue;
        
        if (symbolic) {   		  //-s
        	if (S_ISLNK(file_stat.st_mode)){
        		char buffer[1024];  	// Destino enlace simbolico
            	ssize_t len = readlink(ruta, buffer, sizeof(buffer) - 1);
            	if (len != -1) {  
             	   buffer[len] = '\0';
             	   fprintf(output, "%s -> %s\n", entry->d_name, buffer);
            	} else {		  
             	   fprintf(output, "%s -> ???\n", entry->d_name);
            	}
        	}
        }
        else if (S_ISREG(file_stat.st_mode)) {
            if (exe && !(file_stat.st_mode & S_IXUSR)) continue;  //-x  (si exe activado pero no permisos de ejecucion, se omite)
            fprintf(output, "%s", entry->d_name);
            if (lines) {      									  //-l
                FILE *file = fopen(ruta, "r");
        		if (!file) {
           			perror("Error abriendo el archivo");
           			return;
      			}
        
        		int nLineas = 0;
        		int c;
        		while ((c = fgetc(file)) != EOF) {
          			if (c == '\n') nLineas++;
        		}
        		fclose(file);
        		fprintf(output, ", %d lineas\n", nLineas);
            }
        }
        else if (recursive && S_ISDIR(file_stat.st_mode) && profundidad > 0) {   //-r
            fprintf(output, "%s:\n", ruta);
            list_directory(ruta, lines, exe, symbolic, recursive, profundidad - 1, output);
        }
    }
    
    closedir(dir);
}

// Opción -o (versión larga --output): la salida se volcará en el fichero especificado como argumento. 
	// Internamente se trabajará con un fichero temporal anónimo, que no se hará visible en el sistema de ficheros hasta que no se haya completado el 
	// listado. Sugerencia: usar el flag O_TMPFILE de open, y las llamadas fdopen() y linkat()
	
// Funcion auxiliar para evitar posibles colisiones al hacer visible el fichero anonimo
void generar_nombre_unico(char *nombre_base, char *nombre_final, size_t tam) {
    struct stat st;
    snprintf(nombre_final, tam, "/tmp/%s", nombre_base);

    if (stat(nombre_final, &st) == 0) { // Si el archivo existe, generar un nuevo nombre
        snprintf(nombre_final, tam, "/tmp/%s_%d", nombre_base, rand() % 10000);
    }
}

void redirect_output(char *output_filename, FILE **output) {
    int fd = open("/tmp", O_TMPFILE | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Error creando archivo temporal");
        exit(EXIT_FAILURE);
    }
    
    // Se sobrecarga el output
    *output = fdopen(fd, "w");
    if (!*output) {
        perror("Error con fdopen");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char final_path[1024];
    generar_nombre_unico(output_filename, final_path, sizeof(final_path));

	printf("%s\n", final_path);

    if (linkat(fd, "", AT_FDCWD, final_path, AT_EMPTY_PATH) == -1) {
        perror("Error creando el enlace persistente");
        fclose(*output);
        exit(EXIT_FAILURE);
    }

    fprintf(*output, "Fichero creado y enlazado en: %s\n", final_path);
}

int main(int argc, char *argv[]) {
    int opt;
    int lines = 0, exe = 0, symbolic = 0, recursive = 0, profundidad = -1;  //parametros opciones
    char *filepath = NULL, *dirpath = NULL, *output_file = NULL;   //rutas 
    FILE *output = stdout;
    
    struct option long_options[] = {
        {"lines", no_argument, NULL, 'l'},
        {"exe", no_argument, NULL, 'x'},
        {"symbolic", no_argument, NULL, 's'},
        {"recursive", required_argument, NULL, 'r'},
        {"output", required_argument, NULL, 'o'},
        {0, 0, 0, 0}   // Fin de la lista
    };  
    
    while ((opt = getopt_long(argc, argv, "lf:d:xsr:o:", long_options, NULL)) != -1) {
      switch (opt) {
            case 'l':
                lines = 1;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'd':
                dirpath = optarg;
                break;
            case 'x':
                exe = 1;
                break;
            case 's':
                symbolic = 1;
                break;
            case 'r':
                recursive = 1;
                if (optarg) profundidad = atoi(optarg);
                break;
            case 'o':
                output_file = optarg;
                break;
            default:
                fprintf(stderr, "Uso: %s [-l] -f fichero | -d directorio [-x] [-s] [-r profundidad] [-o archivo_salida]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    if (output_file) {
        redirect_output(output_file, &output);  
    }
    
    if (filepath) {
        list_file(filepath, lines, output);
    } else if (dirpath) {
        list_directory(dirpath, lines, exe, symbolic, recursive, profundidad, output);
    } else {
        fprintf(stderr, "Debe especificar -f fichero o -d directorio\n");
        exit(EXIT_FAILURE);
    }
    if (output_file) fclose(output);
    return 0;
}
