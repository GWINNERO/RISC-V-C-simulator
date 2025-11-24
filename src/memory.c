#include <string.h>
#include "memory.h"
#include "instructions.h"

uint32_t memory[MAX_MEMORY];
int memory_full = 0;

uint32_t get_memory(uint32_t address){
    uint32_t index = address / 4;
    if (index >= MAX_MEMORY) {
        fprintf(stderr,
                "Instruction fetch beyond MAX_MEMORY: pc=0x%08X index=%u (limit=%u)\n",
                address, index, MAX_MEMORY);
        memory_full = 1;
        return 0;   // safe value
    }
    return memory[index];
}

void set_memory(uint32_t address, uint32_t value){
    uint32_t index = address / 4;
    if (index >= MAX_MEMORY) {
        fprintf(stderr,
                "set_memory beyond MAX_MEMORY: addr=0x%08X index=%u (limit=%u)\n",
                address, index, MAX_MEMORY);
        memory_full = 1;
        return;
    }
    memory[index] = value;
}

void write_byte(uint32_t addr, uint8_t value) {
    uint32_t word_index = addr >> 2;
    uint32_t byte_index = addr & 3;

    if (word_index >= MAX_MEMORY) {
        fprintf(stderr,
                "write_byte beyond MAX_MEMORY: addr=0x%08X word_index=%u (limit=%u)\n",
                addr, word_index, MAX_MEMORY);
        memory_full = 1;
        return;
    }

    uint32_t word = memory[word_index];

    uint32_t mask = 0xFFu << (byte_index * 8);
    word = (word & ~mask) | ((uint32_t)value << (byte_index * 8));

    memory[word_index] = word;
}


uint32_t read_byte(uint32_t addr) {
    uint32_t word_index = addr >> 2;
    uint32_t byte_index = addr & 3;

    if (word_index >= MAX_MEMORY) {
        fprintf(stderr,
                "read_byte beyond MAX_MEMORY: addr=0x%08X word_index=%u (limit=%u)\n",
                addr, word_index, MAX_MEMORY);
        memory_full = 1;
        return 0;   // safe value
    }

    uint32_t word = memory[word_index];
    return (word >> (byte_index * 8)) & 0xFF;
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

void bin_dump_registers(const uint32_t register_address[], int REG_COUNT, const char *input_filename) {

/*    // binary printing of registers to console
    for (uint32_t i = 0; i < REG_COUNT; i++) {
        print_binary(register_address[i]);
        printf("\n");
    }
*/
    // Extract file name WITHOUT folders and WITHOUT ".bin"
    const char *base = strrchr(input_filename, '/');
    if (!base) base = strrchr(input_filename, '\\');
    if (!base) base = input_filename;
    else base++; // skip slash

    char clean_name[256];
    strcpy(clean_name, base);

    // remove .bin extension
    char *dot = strrchr(clean_name, '.');
    if (dot) *dot = '\0';

    // Final output path
    char output_path[300];
    snprintf(output_path, sizeof(output_path),
             "results/register_dump_%s.bin", clean_name);

    // Create file
    FILE *out = fopen(output_path, "wb");
    if (!out) {
        printf("Error: could not create %s\n", output_path);
        return;
    }

    // Write registers
    for (uint32_t i = 0; i < REG_COUNT; i++) {
        fwrite(&register_address[i], sizeof(uint32_t), 1, out);
    }

    fclose(out);
    printf("Register dump saved to %s\n", output_path);
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


