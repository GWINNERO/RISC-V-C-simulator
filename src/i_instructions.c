#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

/* ---------- I-type executor (comprehensive) ---------- */
void execute_i_type(uint32_t instr) {
    const uint32_t opcode     = instr & 0x7F;
    const uint32_t ptr_rd     = get_rd(instr);
    const uint32_t ptr_rs1    = get_rs1(instr);
    const uint32_t imm_signed = get_bits(instr, 31, 20);
    const uint32_t funct3     = get_funct3(instr);

    const uint32_t rs1 = get_register(ptr_rs1);
    uint32_t rd = 0;

    // Sign-extend 12-bit immediate
    int32_t imm = (int32_t)((int32_t)(imm_signed << 20) >> 20);

    // Load instructions (opcode 0x03) - dispatch by funct3
    if (opcode == 0x03) {
        uint32_t addr = rs1 + imm;
        uint32_t mem_val = get_memory(addr);
        
        switch (funct3) {
            case 0x0: { // LB - Load Byte (signed)
                int8_t byte_val = (int8_t)(mem_val & 0xFF);
                rd = (uint32_t)byte_val;
                printf("(LB): x[%u] = sign_extend(mem[0x%08X]) = 0x%08X\n", ptr_rd, addr, rd);
                break;
            }
            case 0x1: { // LH - Load Halfword (signed)
                int16_t half_val = (int16_t)(mem_val & 0xFFFF);
                rd = (uint32_t)half_val;
                printf("(LH): x[%u] = sign_extend(mem[0x%08X]) = 0x%08X\n", ptr_rd, addr, rd);
                break;
            }
            case 0x2: { // LW - Load Word
                rd = mem_val;
                printf("(LW): x[%u] = mem[0x%08X] = 0x%08X\n", ptr_rd, addr, rd);
                break;
            }
            case 0x4: { // LBU - Load Byte Unsigned
                rd = mem_val & 0xFF;
                printf("(LBU): x[%u] = zero_extend(mem[0x%08X]) = 0x%08X\n", ptr_rd, addr, rd);
                break;
            }
            case 0x5: { // LHU - Load Halfword Unsigned
                rd = mem_val & 0xFFFF;
                printf("(LHU): x[%u] = zero_extend(mem[0x%08X]) = 0x%08X\n", ptr_rd, addr, rd);
                break;
            }
            default:
                goto illegal;
        }
    }
    // Arithmetic/Logic instructions (opcode 0x13)
    else if (opcode == 0x13) {
        switch (funct3) {
            case 0x0: { // ADDI
                rd = (uint32_t)((int32_t)rs1 + imm);
                printf("(ADDI): %u = %u + %d\n", rd, rs1, imm);
                break;
            }
            case 0x1: { // SLLI
                rd = rs1 << (imm & 0x1F);
                printf("(SLLI): %u = %u << %u\n", rd, rs1, imm & 0x1F);
                break;
            }
            case 0x2: { // SLTI
                rd = ((int32_t)rs1 < imm) ? 1u : 0u;
                printf("(SLTI): %u = %u < %d\n", rd, rs1, imm);
                break;
            }
            case 0x3: { // SLTIU
                rd = (rs1 < (uint32_t)imm) ? 1u : 0u;
                printf("(SLTIU): %u = %u < %u (unsigned)\n", rd, rs1, (uint32_t)imm);
                break;
            }
            case 0x4: { // XORI
                rd = rs1 ^ imm;
                printf("(XORI): %u = %u ^ %d\n", rd, rs1, imm);
                break;
            }
            case 0x5: { // SRLI / SRAI
                uint32_t imm_hi = get_bits(imm_signed, 11, 5);
                if (imm_hi == 0x00) { // SRLI
                    rd = rs1 >> (imm & 0x1F);
                    printf("(SRLI): %u = %u >> %u\n", rd, rs1, imm & 0x1F);
                } else if (imm_hi == 0x20) { // SRAI
                    rd = (uint32_t)(((int32_t)rs1) >> (imm & 0x1F));
                    printf("(SRAI): %u = %u >> %u (arithmetic)\n", rd, rs1, imm & 0x1F);
                } else {
                    goto illegal;
                }
                break;
            }
            case 0x6: { // ORI
                rd = rs1 | imm;
                printf("(ORI): %u = %u | %d\n", rd, rs1, imm);
                break;
            }
            case 0x7: { // ANDI
                rd = rs1 & imm;
                printf("(ANDI): %u = %u & %d\n", rd, rs1, imm);
                break;
            }
            default:
                goto illegal;
        }
    }
    // JALR instruction (opcode 0x67)
    else if (opcode == 0x67) {
        if (funct3 != 0x0) goto illegal;
        // JALR: rd = PC + 4, PC = (rs1 + imm) & ~1
        rd = get_pc() - 4 + 4; // PC after the JALR instruction
        uint32_t target = (rs1 + imm) & ~1u; // Clear LSB for alignment
        printf("(JALR): x[%u] = 0x%08X, PC = 0x%08X\n", ptr_rd, rd, target);
        set_pc(target);
    }
    // FENCE instruction (opcode 0x0F)
    else if (opcode == 0x0F) {
        printf("(FENCE): nop\n");
        // FENCE is a no-op in a single-threaded simulator
    }
    // SYSTEM/ECALL/EBREAK (opcode 0x73)
    else if (opcode == 0x73) {
        // Already handled in main.c before dispatch, but stub here for completeness
        printf("(SYSTEM): 0x%012X\n", imm_signed);
    }
    else {
        goto illegal;
    }

    if (ptr_rd) set_register(ptr_rd, rd);
    return;

    illegal:
        fprintf(stderr, "Illegal I-type encoding (opcode=0x%02X, funct3=0x%X)\n",
                (unsigned)opcode, (unsigned)funct3);
}
