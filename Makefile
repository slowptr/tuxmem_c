# Makefile for creating a static library

# Compiler to use
CC = gcc

# Compiler flags
CFLAGS = -c -Wall

# Name of the static library to create
LIBRARY = tuxmem.a

# Source files for the library
SOURCES = tm_mem.c tm_utils.c

# Object files for the library
OBJECTS = $(SOURCES:.c=.o)

# Target for creating the static library
$(LIBRARY): $(OBJECTS)
	ar rcs $(LIBRARY) $(OBJECTS)

# Rule for creating object files
%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Cleanup rule
clean:
	rm -f $(OBJECTS) $(LIBRARY)
