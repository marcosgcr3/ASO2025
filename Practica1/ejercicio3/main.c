#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "ppm.h"

typedef void (*filtro_func)(ImagenPPM *);

void cargar_y_aplicar_filtro(const char *libname, const char *funcname, ImagenPPM *img) {
    void *handle;
    filtro_func filtro;
    char *error;

    handle = dlopen(libname, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Error al cargar la librería %s: %s\n", libname, dlerror());
        exit(EXIT_FAILURE);
    }

    dlerror(); // Limpiar errores previos

    filtro = (filtro_func) dlsym(handle, funcname);
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "Error al cargar la función %s: %s\n", funcname, error);
        dlclose(handle);
        exit(EXIT_FAILURE);
    }

    filtro(img);

    dlclose(handle);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <imagen_entrada> <imagen_salida> <filtro>\n", argv[0]);
        return EXIT_FAILURE;
    }

    ImagenPPM *img = leer_ppm(argv[1]);
    if (!img) {
        fprintf(stderr, "Error al leer la imagen %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[3], "gris") == 0) {
        cargar_y_aplicar_filtro("./libfiltro_gris.so", "filtro_gris", img);
    } else if (strcmp(argv[3], "sepia") == 0) {
        cargar_y_aplicar_filtro("./libfiltro_sepia.so", "filtro_sepia", img);
    } else {
        fprintf(stderr, "Filtro desconocido: %s\n", argv[3]);
        liberar_ppm(img);
        return EXIT_FAILURE;
    }

    guardar_ppm(argv[2], img);
    liberar_ppm(img);

    return EXIT_SUCCESS;
}