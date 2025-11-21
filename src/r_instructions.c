/**
 * RISC-V Instruction Set C Simulator
 * <p>
 * Example of basic C simulator
 *
 * @author shikoubox
 * @author GWINNERO
 * @author ARTURBARCIJ
 */
#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

#define PROGRAM_SIZE 3
#define REG_SIZE 4

void print_binary(uint32_t value) {
    char binary_str[33]; 
    binary_str[32] = '\0'; 

    for (int i = 31; i >= 0; i--) {
        // set corresponding bit in string
        binary_str[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }

    printf("%s", binary_str);
}

void add(uint32_t rd, uint32_t rs1, uint32_t rs2){
    
    printf("Adding ..\n");
    set_register(rd,get_register(rs1)+get_register(rs2));
    uint32_t rd_val = get_register(rd);
    printf("Register %u has been set to %u\n", rd, rd_val);
}

void execute_r_type(uint32_t instr) {
    //uint32_t opcode = instr & 0x7F;
    uint32_t rd     = (instr >> 7) & 0x1F;
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1    = (instr >> 15) & 0x1F;
    uint32_t rs2    = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25);

    switch (funct3) {
        case 0x0:
            switch(funct7) {
                case 0x0:
                    add(rd, rs1, rs2);
                    break;
                default:
                    printf("no such funct7. \n");
                    break;
                }
            break;
        default:
            printf("no such funct3. \n");
            break;

    }
}