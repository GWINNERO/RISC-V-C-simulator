#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "src/instructions.h"
#include "src/memory.h"

#define REG_COUNT 32


uint32_t pc = 0; // Program counter

uint32_t instruction_memory[0];

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

// Example of using the functions
int main(int argc, char *argv[]) {

    printf("Hello RISC-V World!\n");

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE; // Exit if no file path was provided
    }

    //uint32_t load_file(const char *filename, uint32_t memory[]);
    //load_file(filename, memory[]);
    
    /*
    execute_r_type(0x00FF00F3);
    execute_r_type(0x10FF00F3);

    // Setting some register values
    set_register(1, 10);
    set_register(2, 20);
    
    // Attempting to set x[0], should fail
    set_register(0, 100);
    
    // Retrieving values
    printf("Value in register x[1]: %u\n", get_register(1));
    printf("Value in register x[2]: %u\n", get_register(2));
    */

    return 0;
}
