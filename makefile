# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lgmp -ljpeg -lm
UTILS_DIR = utiles
PRIMOS_DIR = primos
TARGET = primo
UTILS_OBJ = $(UTILS_DIR)/utils.o
PRIMO_OBJ = $(PRIMOS_DIR)/primo.o

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(UTILS_OBJ) $(PRIMO_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(UTILS_OBJ) $(PRIMO_OBJ) $(LDFLAGS)

# Regla para compilar utils.o
$(UTILS_OBJ): $(UTILS_DIR)/utils.c $(UTILS_DIR)/utils.h
	$(CC) $(CFLAGS) -c $(UTILS_DIR)/utils.c -o $(UTILS_OBJ)

# Regla para compilar primo.o
$(PRIMO_OBJ): $(PRIMOS_DIR)/primo.c $(UTILS_DIR)/utils.h
	$(CC) $(CFLAGS) -c $(PRIMOS_DIR)/primo.c -o $(PRIMO_OBJ)

# Regla para limpiar los archivos compilados
clean:
	rm -f $(TARGET) $(UTILS_OBJ) $(PRIMO_OBJ)

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET) -b 1024 -p 0.9