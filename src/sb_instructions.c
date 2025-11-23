#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

void execute_sb_type(uint32_t instruction) {
    uint32_t imm12   = (instruction >> 31) & 0x1;
    uint32_t imm10_5 = (instruction >> 25) & 0x3F;
    uint32_t imm4_1  = (instruction >> 8)  & 0xF;
    uint32_t imm11   = (instruction >> 7)  & 0x1;
    uint32_t imm = (imm12 << 12) | (imm11 << 11) | (imm10_5 << 5) | (imm4_1 << 1);
    int32_t sign_imm = (imm & 0x800) ?(imm | 0xFFFFF000) :(imm & 0xFFF);

    uint32_t funct3 = (instruction >> 12) & 0x7;

    uint32_t ptr_rs1 = (instruction >> 15) & 0x1F;
    uint32_t ptr_rs2 = (instruction >> 20) & 0x1F;
    uint32_t rs1 = get_register(ptr_rs1);
    uint32_t rs2 = get_register(ptr_rs2);
    int32_t sign_rs1 = (int32_t)rs1;
    int32_t sign_rs2 = (int32_t)rs2;

//printf("SB-type decode: funct3=%u imm=0x%08X rs1=%u rs2=%u\n", funct3, imm, rs1, rs2);

    switch (funct3) {
        case 0x0: { // BEQ
            if (rs1 == rs2) {
                uint32_t new_pc = get_pc() - 4 + sign_imm;
                set_pc(new_pc);
                printf("(BEQ) branching to 0x%X, x%u(%u) == x%u(%u)\n", new_pc, ptr_rs1, rs1, ptr_rs2, rs2);
            } else {
                printf("(BEQ) not taken, x%u(%u) == x%u(%u)\n", ptr_rs1, rs1, ptr_rs2, rs2);
            }
          break;
        }
        case 0x1: { // BNE
             if (rs1 != rs2) {
                uint32_t new_pc = get_pc() - 4 + sign_imm;
                set_pc(new_pc);
                printf("(BNE) branching to 0x%X, x%u(%u) != x%u(%u)\n", new_pc, ptr_rs1, rs1, ptr_rs2, rs2);
            } else {
                printf("(BNE) not taken, x%u(%u) != x%u(%u)\n", ptr_rs1, rs1, ptr_rs2, rs2);
            }
            break;
        }
        case 0x4: { // BLT
            if (sign_rs1 < sign_rs2) { 
                uint32_t new_pc = get_pc() - 4 + sign_imm;
                set_pc(new_pc);
                printf("(BLT) branching to 0x%X, x%u(%d) < x%u(%d)\n", new_pc, ptr_rs1, sign_rs1, ptr_rs2, sign_rs2);
            } else {
                printf("(BLT) not taken, x%u(%d) < x%u(%d)\n", ptr_rs1, sign_rs1, ptr_rs2, sign_rs2);
            }
            break;
        } 
        case 0x5: { // BGE 
            if (sign_rs1 >= sign_rs2) { 
                uint32_t new_pc = get_pc() - 4 + sign_imm;
                set_pc(new_pc);
                printf("(BGE) branching to 0x%X, x%u(%d) >= x%u(%d)\n", new_pc, ptr_rs1, sign_rs1, ptr_rs2, sign_rs2);
            } else {
                printf("(BGE) not taken, x%u(%d) >= x%u(%d)\n", ptr_rs1, sign_rs1, ptr_rs2, sign_rs2);
            }
            break;
        }
        case 0x6: { // BLTU
            if (rs1 < rs2) { 
                uint32_t new_pc = get_pc() - 4 + sign_imm;
                set_pc(new_pc);
                printf("(BLTU) branching to 0x%X, x%u(%u) < x%u(%u)\n", new_pc, ptr_rs1, rs1, ptr_rs2, rs2);
            } else {
                printf("(BLTU) not taken, x%u(%u) < x%u(%u)\n", ptr_rs1, rs1, ptr_rs2, rs2);
            }
            break;
        }
        case 0x7: { // BGEU
            if (rs1 >= rs2) { 
                uint32_t new_pc = get_pc() - 4 + sign_imm;
                set_pc(new_pc);
                printf("(BGEU) branching to 0x%X, x%u(%u) >= x%u(%u)\n", new_pc, ptr_rs1, rs1, ptr_rs2, rs2);
            } else {
                printf("(BGEU) not taken, x%u(%u) >= x%u(%u)\n", ptr_rs1, rs1, ptr_rs2, rs2);
            }
            break;
    }
        default:
            printf("Unknown SB funct3 = %u\n", funct3);
            break;
    }

}
/*
void bne(uint32_t rs1, uint32_t rChecking branchmany  ... PASSs2, uint32_t imm) {
    if (get_register(rs1) != get_register(rs2)) {   
        set_pc(get_pc() -4 + imm);
        printf("BNE taken: x[%u]=%u, x[%u]=%u\n", rs1, get_register(rs1), rs2, get_register(rs2));
    } else {
        printf("BNE not taken: x[%u]=%u, x[%u]=%u\n", rs1, get_register(rs1), rs2, get_register(rs2));
    }

void bge(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if ((int32_t)get_register(rs1) >= (int32_t)get_register(rs2)) { 
        set_pc(get_pc() - 4 + imm);
        printf("BGE taken: x[%u]=%d, x[%u]=%d\n", rs1, (int32_t)get_register(rs1), rs2, (int32_t)get_register(rs2));
    } else {
        printf("BGE not taken: x[%u]=%d, x[%u]=%d\n", rs1, (int32_t)get_register(rs1), rs2, (int32_t)get_register(rs2));
    }
}


void bgeu(uint32_t rs1, uint32_t rs2, uint32_t imm) {
    if (get_register(rs1) >= get_register(rs2)) {
        set_pc(get_pc() - 4 + imm);
        printf("BGEU taken: x[%u]=%u, x[%u]=%u\n", rs1, get_register(rs1), rs2, get_register(rs2));
    } else {
        printf("BGEU not taken: x[%u]=%u, x[%u]=%u\n", rs1, get_register(rs1), rs2, get_register(rs2));
    }
}

}*/
