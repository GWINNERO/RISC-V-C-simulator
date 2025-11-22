#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

/* ---------- S-type executor (fixed) ---------- */
void execute_s_type(uint32_t instr) {
    const uint32_t ptr_rd = get_rd(instr);
    const uint32_t ptr_rs1 = get_rs1(instr);
    const uint32_t ptr_rs2 = get_rs2(instr);
    const uint32_t funct3 = get_funct3(instr);

    // S-type imemdiate is split between bits 31-25 and 11-7
    const uint32_t imm_31_25 = get_bits(instr, 31, 25);
    const uint32_t imm_11_7  = get_bits(instr, 11, 7);

    const uint32_t rs1 = get_register(ptr_rs1);
    const uint32_t rs2 = get_register(ptr_rs2);
    uint32_t rd = 0;

    switch (funct3) {
    /*
        case 0x0: { // Store byte
            M[rs1 + imm][0:7] = regs[rs2] & 0xFF;
            a[rd1 + imm_31_25][0:7] = (regs[rs2] >> 8) & 0xFF;
            break;}
        case 0x1: { // Store half
            M[rs1 + imm][0:15] = regs[rs2] & 0xFFFF;
            break;}
        case 0x2: { // Store word
            M[rs1 + imm][0:31] = regs[rs2];
            break;}
         */   
        default:
            goto illegal;
    }

    if (ptr_rd) set_register(ptr_rd, rd);   // keep x0 hardwired to 0
    printf("S type execute, set %u to %u\n", ptr_rd, rd);
    return;

    illegal:
        fprintf(stderr, "Illegal S-type encoding (funct3=0x%X)\n",
                (unsigned)funct3);

}   