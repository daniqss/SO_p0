CC = gcc
CFLAGS = -Wall -Werror
EXECUTABLE = main
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

#.PHONY: all clean run

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
    $(CC) $(CFLAGS) -o $@ $^

%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $<

run: $(EXECUTABLE)
	valgrind --leak-check=full ./$(EXECUTABLE)

clean:
    rm -f $(OBJECTS) $(EXECUTABLE)
