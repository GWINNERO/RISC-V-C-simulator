#include "memory.h"

uint32_t count = 0;
uint32_t buffer[];

uint32_t load_file(const char *filename, uint32_t memory[]) {
    FILE *file = fopen(filename, "rb"); // Open file in binary mode
    if (!file) { // if opening file fails
        printf("Error: Could not open file %s\n", filename);
        return (uint32_t)-1; // Return max uint32 val to indicate error
    }
}

while (fread(&memory[count], sizeof(uint32_t), 1, file) == 1) {
        count++;
}

    fclose(file);
    return count; // Return number of instructions loaded



if (count >= MAX_INSTRUCTIONS) {
        printf("Error: Instruction memory overflow.\n");
        fclose(file);
        return -1;
    }