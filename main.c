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


#define REG_COUNT 32 // Register count

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

    bin_dump_registers(x, REG_COUNT, argv[1]);



 /* Create the file */
    int a = 5;
    FILE *fp = fopen ("file.bin", "wb");
    if (fp == NULL)
      return -1;
    fwrite (&a, sizeof (a), 1, fp);
    fclose (fp);

    exit(0);
}
