/**
 * RISC-V Instruction Set C Simulator
 * <p>
 * R type instructions 
 *
 * @author shikoubox
 * @author GWINNERO
 * @author ARTURBARCIJ
 */

#include <stdio.h>
#include <stdint.h>
#include "instructions.h"


/* ---------- R-type executor (fixed) ---------- */
void execute_r_type(uint32_t instr) {
    const uint32_t ptr_rd     = get_rd(instr);
    const uint32_t ptr_rs1    = get_rs1(instr);
    const uint32_t ptr_rs2    = get_rs2(instr);
    const uint32_t funct3 = get_funct3(instr);
    const uint32_t funct7 = get_funct7(instr);

    const uint32_t rs1 = get_register(ptr_rs1); // a
    const uint32_t rs2 = get_register(ptr_rs2); // b
    uint32_t rd = 0;
    switch (funct3) {
        case 0x0: { // ADD / SUB
            switch (funct7) {
                case 0x00: {
                    rd = (uint32_t)((int32_t)rs1 + (int32_t)rs2);
                    printf("(ADD): %u = %u + %u\n",rd,rs1,rs2);
                    break;
                } // ADD
                case 0x20: {
                    rd = (uint32_t)((int32_t)rs1 - (int32_t)rs2);
                    printf("(SUB): %u = %u - %u\n",rd,rs1,rs2);
                    break;
                }
                default: {
                    goto illegal;
                }
            } break;
        }
        case 0x1: { // SLL
            if (funct7 != 0x00) goto illegal;
            rd = rs1 << (rs2 & 0x1F);
            printf("(SLL): %u = %u << (%u & 0x1F)\n",rd,rs1,rs2);
            break;
        }
        case 0x2: { // SLT (signed)
            if (funct7 != 0x00) goto illegal;
            rd = ((int32_t)rs1 < (int32_t)rs2) ? 1u : 0u;
            printf("(SLT (signed)): %u = %u < %u\n",rd,rs1,rs2);
            break;
        }
        case 0x3: { // SLTU (unsigned)
            if (funct7 != 0x00) goto illegal;
            rd = (rs1 < rs2) ? 1u : 0u;
            printf("(SLTU (unsigned)): %u = %u < %u\n",rd,rs1,rs2);
            break;
        }
        case 0x4: { // XOR
            if (funct7 != 0x00) goto illegal;
            rd = rs1 ^ rs2;
            printf("(XOR): %u = %u ^ %u\n",rd,rs1,rs2);
            break;
        }
        case 0x5: { // SRL / SRA
            switch (funct7) {
                case 0x00: { // SRL
                    rd = rs1 >> (rs2 & 0x1F);
                    printf("(SRL): %u = %u >> (%u & 0x1F)\n",rd,rs1,rs2);
                    break;
                }
                case 0x20: { // SRA
                    rd = (uint32_t)(((int32_t)rs1) >> (rs2 & 0x1F));
                    printf("(SRA): %u = %u >> (%u & 0x1F)\n",rd,rs1,rs2);
                    break; 
                }
                default: 
                    goto illegal;
            } break;
        }
        case 0x6: { // OR
            if (funct7 != 0x00) goto illegal;
            rd = rs1 | rs2;
            printf("(OR): %u = %u | %u\n",rd,rs1,rs2);
            break;
        }
        case 0x7: { // AND
            if (funct7 != 0x00) goto illegal;
            rd = rs1 & rs2;
            printf("(AND): %u = %u & %u\n",rd,rs1,rs2);
            break;
        }
        default:
            goto illegal;
    }

    if (ptr_rd) set_register(ptr_rd, rd);   // keep x0 hardwired to 0
    return;

    illegal:
        fprintf(stderr, "Illegal R-type encoding (funct7=0x%02X, funct3=0x%X)\n",
                (unsigned)funct7, (unsigned)funct3);
}
