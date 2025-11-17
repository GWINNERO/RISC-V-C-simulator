#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "src/instructions.h"
#include "src/memory.h"

#define REG_COUNT 32

uint32_t memory[MAX_INSTRUCTIONS]; // Instruction memory start at 0
                                   // Stack memory start at 1 MB (262144)

uint32_t pc = 0; // Program counter

uint32_t x[REG_COUNT]; // Array holding 32 registers

uint32_t imm = 0; // Global immediate value

void enqueue(uint32_t instruction){
    // Program enqueue an instruction
    // Make address be 1 fourth
}

uint32_t dequeue(){
    return instruction_memory[pc];
    pc+=4;
    // Address be four
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

// Test loading file
/*
int main(void) {

    if (instruction_count == (uint32_t)-1) {
        printf("Program load failed.\n");
        return (uint32_t)-1;
    }

    printf("Loaded %u instructions.\n", instruction_count);

    for (uint32_t i = 0; i < instruction_count; i++) {
        printf("instruction memory[%u] = 0x%08X\n", i, memory[i]);
    }

    return 0;
}
*/
// End test loading file


// Example of using the functions
int main(int argc, char *argv[]) {

    printf("RISC-V 32 C simulator!\n");


    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        fprintf(stderr, "Execute a binary file with riscv instructions!");
        return EXIT_FAILURE; // Exit if no file path was provided
    }

    //uint32_t instruction_count = load_file("test_files\\T1\\addlarge.bin", instruction_memory);
    uint32_t instruction_count = load_file(argv[1], memory);
    if(instruction_count == 0xFFFFFFFF){
        printf("Error from load_file() received");
    }
    

    for (int i = 0; i <= instruction_count; i++) {
        printf("0x%X\n", memory[i]);
    }


    //uint32_t load_file(const char *filename, uint32_t memory[]);
    //load_file(filename, memory[]);
    
    //execute_r_type(0x00FF00F3);
    //execute_r_type(0x10FF00F3);

    // Setting some register values
    //set_register(1, 10);
    //set_register(2, 20);
    
    // Attempting to set x[0], should fail
    //set_register(0, 100);
    
    // Retrieving values
    //printf("Value in register x[1]: %u\n", get_register(1));

     return 0;
  }
