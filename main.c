#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#endif


#include "src/instructions.h"
#include "src/memory.h"


#define REG_COUNT 32            // Register count

uint32_t memory[MAX_MEMORY];    // Instruction memory start at 0
                                // Stack memory start at 1 MB (262144)

uint32_t pc = 0; // Program counter

uint32_t x[REG_COUNT]; // Array holding 32 registers

uint32_t imm = 0; // Global immediate value


bool running = true;
                  
void set_pc(uint32_t value) {
    pc = value;
}

uint32_t get_pc(){
    return pc;
}

void set_register(uint32_t reg, uint32_t value) {
    if (reg == 0) {
        printf("Error: Register x[0] is read-only.\n");
    } else if (reg >= 1 && reg < REG_COUNT) {
        x[reg] = value;
    } else {
        printf("Error: Invalid register index %d.\n", reg);
    }
}

uint32_t get_register(uint32_t reg) {
    if (reg >= 0 && reg < REG_COUNT) {
        return x[reg];
    } else {
        printf("Error: Invalid register index %d.\n", reg);
        return 0; // Return 0 for invalid index
    }
}

uint32_t get_memory(uint32_t address){
    // Return a 32-bit value where the requested address' byte is in the lowest byte.
    // This allows callers that read bytes/halves using masks to work correctly
    // even when the address is not word-aligned.
    uint32_t word_index = address / 4;
    uint32_t byte_offset = address % 4;
    uint32_t word = memory[word_index];
    // Shift the requested byte into lowest position. For LW callers that expect
    // full word at aligned addresses, byte_offset will be 0 and the whole word is returned.
    return (word >> (8 * byte_offset));
}

void set_memory(uint32_t address, uint32_t value){
    uint32_t word_index = address / 4;
    uint32_t byte_offset = address % 4;

    // Heuristic: caller passes only the relevant bytes for SB/SH (masked to 0xFF/0xFFFF).
    // Detect intended store width by value range.
    if (value <= 0xFFu) {
        // store byte
        uint32_t mask = ~(0xFFu << (8 * byte_offset));
        memory[word_index] = (memory[word_index] & mask) | ((value & 0xFFu) << (8 * byte_offset));
    } else if (value <= 0xFFFFu) {
        // store halfword (may cross word boundary if unaligned)
        if (byte_offset <= 2) {
            uint32_t mask = ~(0xFFFFu << (8 * byte_offset));
            memory[word_index] = (memory[word_index] & mask) | ((value & 0xFFFFu) << (8 * byte_offset));
        } else {
            // split across two words: write low byte here and high byte into next word
            uint32_t low = value & 0xFFu;
            uint32_t high = (value >> 8) & 0xFFu;
            uint32_t mask = ~(0xFFu << (8 * byte_offset));
            memory[word_index] = (memory[word_index] & mask) | (low << (8 * byte_offset));
            // next word
            uint32_t next_mask = ~0xFFu;
            memory[word_index + 1] = (memory[word_index + 1] & next_mask) | high;
        }
    } else {
        // store word (assume aligned in tests). If unaligned, split across two words.
        if (byte_offset == 0) {
            memory[word_index] = value;
        } else {
            // split into bytes across current and next word
            for (int i = 0; i < 4; ++i) {
                uint32_t b = (value >> (8 * i)) & 0xFFu;
                uint32_t idx = word_index + ((byte_offset + i) / 4);
                uint32_t off = (byte_offset + i) % 4;
                uint32_t m = ~(0xFFu << (8 * off));
                memory[idx] = (memory[idx] & m) | (b << (8 * off));
            }
        }
    }
}

bool execute_instruction(){
    uint32_t memory_address = pc/4;
    printf("[%3u] ", pc);
    pc+=4; //increase pc with 4

    uint32_t instruction = memory[memory_address];
    printf("[0x%08X] ", instruction);
    // end program if ECALL met
    if(instruction == 0x73 )
    {
        printf("ecall 10\n");
        return false;
    }

    dispatch_type(instruction);
    return true;
}

// Example of using the functions
int main(int argc, char *argv[]) {

    printf("RISC-V 32 C simulator!\n");


    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        fprintf(stderr, "Execute a binary file with riscv instructions!");
        return EXIT_FAILURE; // Exit if no file path was provided
    }

    uint32_t instruction_count = load_file(argv[1], memory);
    if(instruction_count == 0xFFFFFFFF){
        printf("Error from load_file() received");
    }
    

    //for (int i = 0; i <= instruction_count; i++) {
    //    printf("Instruction[%u]: 0x%08X\n", i, memory[i]);
    //}



    
    while(running){
        running = execute_instruction();
    }



    // Print registers
    // Retrieving values
/*
    for (int i = 0; i < REG_COUNT; i++){
        printf("Value in register x[%2u]: %u\n", i, get_register(1));
    }
*/

    bin_dump_registers(x, REG_COUNT);


 /* Create the file */
    int a = 5;
    FILE *fp = fopen ("file.bin", "wb");
    if (fp == NULL)
      return -1;
    fwrite (&a, sizeof (a), 1, fp);
    fclose (fp);

    exit(0);
}
