#include "memory.h"
#include "instructions.h"

uint32_t load_file(const char *filename, uint32_t memory[]) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return (uint32_t)-1;
    }

    uint32_t count = 0;
    uint32_t instruction = 0;

    while (fread(&instruction, sizeof(uint32_t), 1, file) == 1) {
        if (count >= MAX_MEMORY) {
            printf("Error: Instruction memory overflow.\n");
            fclose(file);
            return (uint32_t)-1;
        }

        memory[count] = instruction;
        count++;
    }

    printf("Opened the file %s containing %u instructions\n", filename, count);

    fclose(file);
    return count;
}

void bin_dump_registers(const uint32_t register_address[], int REG_COUNT) {
    for (uint32_t i = 0; i < REG_COUNT; i++) {
        print_binary(register_address[i]);
        printf("\n");
    }
}
