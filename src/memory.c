#include "memory.h"

uint32_t load_file(const char *filename, uint32_t memory[]) {
    FILE *file = fopen(filename, "rb"); // Open file in binary mode
    if (!file) { // If opening file fails
        printf("Error: Could not open file %s\n", filename);
        return (uint32_t)-1; // Use 0xFFFFFFFF (max uint32_t value) to indicate error
    }

    uint32_t count = 0;          // How many instructions are loaded
    uint32_t instruction = 0;    // Temporary storage for one word (32 bits)

    // Read 32 bits (4 bytes) at a time, until fread stops returning 1
    while (fread(&instruction, sizeof(uint32_t), 1, file) == 1) {

        // Check if we exceed memory size
        if (count >= MAX_INSTRUCTIONS) {
            printf("Error: Instruction memory overflow.\n");
            fclose(file);
            return (uint32_t)-1; // Use 0xFFFFFFFF (max uint32_t value) to indicate error
        }

        // Store word instruction into the memory array
        memory[count] = instruction;

        count++;
    }
    printf("Opened the file %s containing %u instructions\n", filename, count);

    fclose(file);
    return count; // Return the number of words (32-bit instructions) loaded from the file
}
