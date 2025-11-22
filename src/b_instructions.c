#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

/* ---------- B-type executor (stub) ---------- */
void execute_b_type(uint32_t x){ 
    const uint32_t ptr_rd     = get_rd(x);
    const uint32_t ptr_rs1    = get_rs1(x);
    const uint32_t ptr_rs2    = get_rs2(x);
    const uint32_t funct3 = get_funct3(x);
    const uint32_t funct7 = get_funct7(x);
    const uint32_t imm_31_25    = get_bits(x, 31, 25);
    const uint32_t  imm_11_7 = get_bits(x, 11,7);
    
    const uint32_t rs1 = get_register(ptr_rs1);
    const uint32_t rs2 = get_register(ptr_rs2);
    uint32_t rd = 0;

    switch (funct3)
    {
    case 0x0: // BEQ
        if (rs1 == rs2) {
            // PC = PC + imm
        }
        break;
    case 0x1: // BNE
        if (rs1 != rs2) {
            // PC = PC + imm
        }
        break;
    case 0x4: // BLT
        if ((int32_t)rs1 < (int32_t)rs2) {
            // PC = PC + imm
        }
        break;
    case 0x5: // BGE
        if ((int32_t)rs1 >= (int32_t)rs2) {
            // PC = PC + imm
        }
        break;
    case 0x6: // BLTU
        if (rs1 < rs2) {
            // PC = PC + imm
        }
        break;
    case 0x7: // BGEU
        if (rs1 >= rs2) {
            // PC = PC + imm
        }
        break;

    default:
        break;
    }
    

    if (ptr_rd) set_register(ptr_rd, rd);   // keep x0 hardwired to 0
    printf("B type execute, set %u to %u\n", ptr_rd, rd);
    return;

    (void)x; puts("B-type stub"); }
