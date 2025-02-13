# makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBNAME = libppmaso
STATIC_LIB = $(LIBNAME).a
DYNAMIC_LIB = $(LIBNAME).so
OBJS = filtro_gris.o filtro_sepia.o ppm.o
TARGET_STATIC = appstatic
TARGET_DYNAMIC = appdynamic

all: $(TARGET_STATIC) $(TARGET_DYNAMIC)

# Compilar los archivos objeto
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Crear la librería estática
$(STATIC_LIB): $(OBJS)
    ar rcs $@ $^

# Crear la librería dinámica
$(DYNAMIC_LIB): $(OBJS)
    $(CC) -shared -o $@ $^

# Compilar el ejecutable estático
$(TARGET_STATIC): main.o $(STATIC_LIB)
    $(CC) $(CFLAGS) -o $@ main.o -L. -lppmaso

# Compilar el ejecutable dinámico
$(TARGET_DYNAMIC): main.o $(DYNAMIC_LIB)
    $(CC) $(CFLAGS) -o $@ main.o -L. -lppmaso -Wl,-rpath,.

# Limpiar archivos generados
clean:
    rm -f *.o $(STATIC_LIB) $(DYNAMIC_LIB) $(TARGET_STATIC) $(TARGET_DYNAMIC)

.PHONY: all clean