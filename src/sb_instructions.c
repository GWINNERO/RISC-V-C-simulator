#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

uint32_t execute_sb_type(uint32_t instruction) {
    uint32_t imm12   = (instruction >> 31) & 0x1;
    uint32_t imm10_5 = (instruction >> 25) & 0x3F;
    uint32_t imm4_1  = (instruction >> 8)  & 0xF;
    uint32_t imm11   = (instruction >> 7)  & 0x1;
    uint32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);

    if (imm12)
        imm |= 0xFFFFE000; // sign extend

    return (uint32_t)imm;


    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;

    uint32_t imm = decode_sb_imm(instruction);

    switch (funct3) {
        case 0x0: beq(rs1, rs2, imm); break;
        case 0x1: bne(rs1, rs2, imm); break;
        case 0x4: blt(rs1, rs2, imm); break;
        case 0x5: bge(rs1, rs2, imm); break;
        case 0x6: bltu(rs1, rs2, imm); break;
        case 0x7: bgeu(rs1, rs2, imm); break;
        default:
            printf("Unknown SB funct3 = %u\n", funct3);
            break;
    }

}


void beq(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if (get_register(rs1) == get_register(rs2))
        set_pc(get_pc() + imm);
}

void bne(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if (get_register(rs1) != get_register(rs2))
        set_pc(get_pc() + imm);
}

void blt(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if ((int32_t)get_register(rs1) < (int32_t)get_register(rs2))
        set_pc(get_pc() + imm);
}

void bge(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if ((int32_t)get_register(rs1) >= (int32_t)get_register(rs2))
        set_pc(get_pc() + imm);
}

void bltu(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if (get_register(rs1) < get_register(rs2))
        set_pc(get_pc() + imm);
}

void bgeu(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if (get_register(rs1) >= get_register(rs2))
        set_pc(get_pc() + imm);
}

