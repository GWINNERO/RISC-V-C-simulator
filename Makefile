#Define the compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRC = main.c src/memory.c src/r_instructions.c

# Output
OUTPUT = program

# Default rule to build the program
all: $(OUTPUT)

# Link everything together
$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC)

# Clean up build files
clean:
	rm -f $(OUTPUT)

# Run the program
run: all
	./$(OUTPUT)

