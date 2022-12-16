# Specify the target file and the sources that it depends on
main: main.c tm_mem.c tm_utils.c
	gcc -Wall main.c tm_mem.c tm_utils.c -o tuxmem

# Specify the target for cleaning up the project directory
clean:
	rm -f main

# Specify the target for the "all" operator and its dependencies
all: main clean

