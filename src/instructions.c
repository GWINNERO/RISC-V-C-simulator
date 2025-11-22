#include <stdio.h>
#include <stdint.h>
#include "instructions.h"


uint32_t get_bits(uint32_t x, int hi, int lo) {
    uint32_t val = 0xFFFFFFFF;
    return (x >> lo) & (val >> (32 - (hi - lo + 1)));
}

void execute_i_type(uint32_t instruction) {
    uint32_t imm = get_bits(instruction, 31, 20);
    uint32_t rs1 = get_bits(instruction, 19, 15);
    uint32_t funct3 = get_bits(instruction, 14, 12);
    uint32_t rd = get_bits(instruction, 11, 7);
    uint32_t opcode = get_bits(instruction, 6, 0);
    printf("Called I_type function\n");
}

void execute_s_type(uint32_t instruction) {
    uint32_t imm11_5 = get_bits(instruction, 31, 25);
    uint32_t rs2 = get_bits(instruction, 24, 20);
    uint32_t rs1 = get_bits(instruction, 19, 15);
    uint32_t funct3 = get_bits(instruction, 14, 12);
    uint32_t imm4_0 = get_bits(instruction, 11, 7);
    uint32_t opcode = get_bits(instruction, 6, 0);
    printf("Called S_type function\n");
}

void execute_sb_type(uint32_t instruction) {
    uint32_t imm12 = get_bits(instruction, 31, 31);
    uint32_t imm10_5 = get_bits(instruction, 30, 25);
    uint32_t rs2 = get_bits(instruction, 24, 20);
    uint32_t rs1 = get_bits(instruction, 19, 15);
    uint32_t funct3 = get_bits(instruction, 14, 12);
    uint32_t imm4_1 = get_bits(instruction, 11, 8);
    uint32_t imm11 = get_bits(instruction, 7, 7);
    uint32_t opcode = get_bits(instruction, 6, 0);
    printf("Called SB_type function\n");
}

void execute_u_type(uint32_t instruction) {
    uint32_t imm31_12 = get_bits(instruction, 31, 12);
    uint32_t rd = get_bits(instruction, 11, 7);
    uint32_t opcode = get_bits(instruction, 6, 0);
    printf("Called U_type function\n");
}

void execute_uj_type(uint32_t instruction) {
    uint32_t imm20 = get_bits(instruction, 31, 31);
    uint32_t imm10_1 = get_bits(instruction, 30, 21);
    uint32_t imm11 = get_bits(instruction, 20, 20);
    uint32_t imm19_12 = get_bits(instruction, 19, 12);
    uint32_t rd = get_bits(instruction, 11, 7);
    uint32_t opcode = get_bits(instruction, 6, 0);
    printf("Called UJ_type function\n");
}

// dispatcher
void dispatch_type(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F; // last 7 bits

    switch (opcode) {
        case 0b0110011: // R-type
            execute_r_type(instruction);
            break;

        case 0b0000011: case 0b0001111: case 0b0010011: case 0b1100111: case 0b1110011: // I-type group
            execute_i_type(instruction);
            break;

        case 0b0100011: // S-type
            execute_s_type(instruction);
            break;

        case 0b1100011: // SB-type
            execute_sb_type(instruction);
            break;

        case 0b0010111: case 0b0110111: // U-type
            execute_u_type(instruction);
            break;

        case 0b1101111: // UJ-type
            execute_uj_type(instruction);
            break;

        default:
            printf("Non existent opcode ERROR: 0x%02X\n", (unsigned)opcode);
            break;
    }
}

// main
/*main() {
    uint32_t instruction = 0b0010011; // example R-type
    dispatch_type(instruction);
    return 0;
}*/
