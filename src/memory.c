#include "memory.h"
#include "instructions.h"

uint32_t memory[MAX_MEMORY];

uint32_t get_memory(uint32_t address){
    return memory[address/4];
}

void set_memory(uint32_t address, uint32_t value){
    memory[address/4] = value;
}

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

    // binary printing of registers to console
    for (uint32_t i = 0; i < REG_COUNT; i++) {
        print_binary(register_address[i]);
        printf("\n");
    }

    // open results folder to write the registers into a .bin file
    FILE *out = fopen("results/register_dump.bin", "wb");
    if (!out) {
        printf("error cound not execute\n");
        return;
    }

    // write each register as a 32-bit value into .bin file
    for (uint32_t i = 0; i < REG_COUNT; i++) {
        fwrite(&register_address[i], sizeof(uint32_t), 1, out);
    }

    fclose(out);
    printf("Register dump saved to results/register_dump.bin\n");
}

// sign extender
int32_t sign_extender(uint32_t value, int bits) {
    int shift = 32 - bits;
    return ((int32_t)(value << shift)) >> shift;
}


// Memory store helpers
void store_word(uint32_t address, uint32_t value) {
    memory[address / 4] = value;
}

void store_half(uint32_t address, uint32_t value) {
    uint32_t index = address / 4;

    // Only store lowest 16 bits, set to 32 bits
    memory[index] = (uint32_t)(value & 0xFFFF);
}

void store_byte(uint32_t address, uint32_t value) {
    uint32_t index = address / 4;

    // Only store lowest 8 bits, set to 32 bits
    memory[index] = (uint32_t)(value & 0xFF);
}


