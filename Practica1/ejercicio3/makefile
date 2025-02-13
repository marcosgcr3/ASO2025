# makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBNAME = libppmaso
STATIC_LIB = $(LIBNAME).a
DYNAMIC_LIB = $(LIBNAME).so
FILTER_GRIS_LIB = libfiltro_gris.so
FILTER_SEPIA_LIB = libfiltro_sepia.so
OBJS = ppm.o
FILTER_GRIS_OBJ = filtro_gris.o
FILTER_SEPIA_OBJ = filtro_sepia.o
TARGET_STATIC = appstatic
TARGET_DYNAMIC = appdynamic

all: $(TARGET_STATIC) $(TARGET_DYNAMIC)

# Compilar los archivos objeto
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Crear la librería estática
$(STATIC_LIB): $(OBJS) $(FILTER_GRIS_OBJ) $(FILTER_SEPIA_OBJ)
    ar rcs $@ $^

# Crear la librería dinámica principal
$(DYNAMIC_LIB): $(OBJS)
    $(CC) -shared -o $@ $^

# Crear la librería dinámica para filtro_gris
$(FILTER_GRIS_LIB): $(FILTER_GRIS_OBJ)
    $(CC) -shared -o $@ $^

# Crear la librería dinámica para filtro_sepia
$(FILTER_SEPIA_LIB): $(FILTER_SEPIA_OBJ)
    $(CC) -shared -o $@ $^

# Compilar el ejecutable estático
$(TARGET_STATIC): main.o $(STATIC_LIB)
    $(CC) $(CFLAGS) -o $@ main.o -L. -lppmaso

# Compilar el ejecutable dinámico
$(TARGET_DYNAMIC): main.o $(DYNAMIC_LIB) $(FILTER_GRIS_LIB) $(FILTER_SEPIA_LIB)
    $(CC) $(CFLAGS) -o $@ main.o -L. -lppmaso -ldl -Wl,-rpath,.

# Limpiar archivos generados
clean:
    rm -f *.o $(STATIC_LIB) $(DYNAMIC_LIB) $(FILTER_GRIS_LIB) $(FILTER_SEPIA_LIB) $(TARGET_STATIC) $(TARGET_DYNAMIC)

.PHONY: all clean