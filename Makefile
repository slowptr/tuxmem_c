# Makefile for my_library

# Compiler and linker
CC = gcc

# Compiler flags
CFLAGS = -g -O2 -Wall -Wextra

# Library source files
SRCS = $(wildcard src/*.c)

# Library object files
OBJS = $(SRCS:.c=.o)

# Library name
TARGET = tuxmem.a

# Test source files
TEST_SRCS = $(wildcard test/*.c)

# Test executables
TESTS = $(TEST_SRCS:.c=)

# Include directories
INCLUDES = -Iinclude

# Library dependencies
LDLIBS =

# Build the library
$(TARGET): $(OBJS)
	ar rcs $@ $^

# Compile the test executables
$(TESTS): $(TARGET)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $@.c $(TARGET) $(LDLIBS)

# Run the tests
test: $(TESTS)
	for test in $(TESTS); do \
		./$$test; \
	done

# Clean up
clean:
	rm -f $(OBJS) $(TARGET) $(TESTS)

.PHONY: test clean

