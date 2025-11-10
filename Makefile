#Define the compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRC = ISA_sim.c

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

