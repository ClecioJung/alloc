# ----------------------------------------
# Project definitions
# ----------------------------------------

# Name of the project
EXEC = alloc

# ----------------------------------------
# Compiler and linker definitions
# ----------------------------------------

# Compiler and linker
CC = gcc

# Flags for compiler
CFLAGS = -W -Wall -Wextra -pedantic -std=c99 -Os -DDEBUG

# ----------------------------------------
# Compilation and linking rules
# ----------------------------------------

all: $(EXEC)

$(EXEC): alloc.h alloc.c main.c
	$(CC) $(CFLAGS) $(filter %.c %.s %.o,$^) -o $@

# ----------------------------------------
# Script rules
# ----------------------------------------

memcheck:
	valgrind --tool=memcheck ./$(EXEC)

clean:
	rm -rf $(EXEC)

.PHONY: all clean

# ----------------------------------------
