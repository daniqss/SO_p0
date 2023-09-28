#Compilador
CC = gcc

# Nombre del ejecutable
TARGET = p0

# Fuentes y objetos
SRCS = p0.c command_list.c file_list.c
OBJS = $(SRCS:.c=.o)

#Opciones del compilador
CFLAGS = -Wall

# Regla predeterminada
all: $(TARGET)

#Compilación
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
%.c:
# Limpieza de archivos generados
clean:
	rm -f $(OBJS) $(TARGET)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Ejecutar Valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

.PHONY: all clean valgrind run