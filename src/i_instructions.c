#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

/* ---------- I-type executor (fixed) ---------- */
void execute_i_type(uint32_t instr) {
    const uint32_t opcode     = instr & 0x7F;
    const uint32_t ptr_rd     = get_rd(instr);
    const uint32_t ptr_rs1    = get_rs1(instr);
    const uint32_t imm    = get_bits(instr, 31, 20);
    const uint32_t funct3 = get_funct3(instr);

    const uint32_t rs1 = get_register(ptr_rs1); // a
    uint32_t rd = 0;

    switch (funct3) {
        case 0x0: { // ADDI
            switch(opcode){
                case 0x13: {
                    rd = (uint32_t)((int32_t)rs1 + (int32_t)imm);
                    printf("(ADDI): %u = %u + %u\n",rd,rs1,imm);
                    break;
                }
                case 0x03: { // LB
                    // Calculate memory address
                    uint32_t effective_address = rs1 + imm;
                    uint8_t rd = get_memory(effective_address);
                    // Sign-extend the byte to 32-bits
                    if (rd & 0x80) { 
                       rd = rd | 0xFFFFFF00;
                    }
                    // For debugging: print the result
                    printf("(LB): Loaded byte from address 0x%08X into x[%d] = 0x%08X\n",
                           effective_address, ptr_rd, rd);
                    //printf("(LB): %u = %u + %u\n",rd,rs1,imm);
                    break;
                }
                default:
                    goto illegal;
            }
        }
        case 0x1: { // SLLI
            switch(opcode){
                case 0x13: {
            if (funct7 != 0x00) goto illegal;
            rd = rs1 << (imm & 0x1F);
            printf("(SLLI): %u = %u << %u\n",rd,rs1,imm);
            break;
        }
        case 0x2: { // SLTI
            rd = ((int32_t)rs1 < (int32_t)imm) ? 1u : 0u;
            printf("(SLTI): %u = %u < %u\n",rd,rs1,imm);
            break;
        }
        case 0x3: { // SLTIU
            rd = (rs1 < imm) ? 1u : 0u;
            printf("(SLTIU): %u = %u < %u\n",rd,rs1,imm);
            break;
        }
        case 0x4: { // XORI
            rd = rs1 ^ imm;
            printf("(XORI): %u = %u ^ %u\n",rd,rs1,imm);
            break;
        }
        case 0x5: { // SRLI / SRAI
            uint32_t imm_hi = get_bits(imm, 11, 5);
            switch (imm_hi) {
                case 0x00: {
                    rd = rs1 >> (imm & 0x1F);
                    printf("(SRLI): %u = %u >> %u\n",rd,rs1,imm);
                    break;                          // SRLI
                }                                                                 //
                case 0x20: {
                    rd = (uint32_t)(((int32_t)rs1) >> (imm & 0x1F));
                    printf("(SRAI): %u = %u >> %u\n",rd,rs1,imm);
                    break;   // SRAI
                }
                default:   goto illegal;
            }
            break;
        }
        case 0x6: { // ORI
            rd = rs1 | imm;
            printf("(ORI): %u = %u | %u\n",rd,rs1,imm);
            break;
        }
        case 0x7: { // ANDI
            rd = rs1 & imm;
            printf("(ANDI): %u = %u & %u\n",rd,rs1,imm);
            break;
        }
        default:
            goto illegal;
    }

    if (ptr_rd) set_register(ptr_rd, rd);   // keep x0 hardwired to 0
    return;

    illegal:
        fprintf(stderr, "Illegal I-type encoding (funct7=0x%02X, funct3=0x%X)\n",
                (unsigned)funct7, (unsigned)funct3);
}
