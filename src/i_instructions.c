#include <stdio.h>
#include <stdint.h>
#include "instructions.h"
#include "memory.h"

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
                    uint32_t temp = effective_address % 4;

                    uint32_t word = get_memory(effective_address-temp);
                    uint32_t rd = (word >> (temp * 8)) & 0xFF;

                    // Sign-extend the byte to 32-bits
                    if (rd & 0x80) { 
                       rd = rd | 0xFFFFFF00;
                    }
                    // For debugging: print the result
                    printf("(LB): Loaded byte from address 0x%08X into x[%d] = 0x%08X\n",
                           effective_address, ptr_rd, rd);
                    break;
                }
                case 0x67: {
                    // jalr
                    set_register(rd, get_pc());
                    set_pc(rs1+imm);
                    printf("(JALR) (rd)%u (rs1)%u (imm)%u\n",rd,rs1,imm);
                    break;                                                     
                }
                default:
                    goto illegal;
            }
            break;
        }
        case 0x1: { 
            switch(opcode){
                case 0x13: {
                        // SLLI
                        rd = rs1 << (imm & 0x1F);
                        printf("(SLLI): %u = %u << %u\n",rd,rs1,imm);
                        break;
                      }
                case 0x3: {
                    // LH load half word
                    // Calculate memory address
                    uint32_t effective_address = rs1 + imm;
                    uint32_t temp = effective_address % 4;

                    uint32_t word = get_memory(effective_address-temp);
                    uint32_t rd = (word >> (temp * 8)) & 0xFFFF;

                    // Sign-extend the byte to 32-bits
                    if (rd & 0x8000) { 
                       rd = rd | 0xFFFF0000;
                    }
                    // For debugging: print the result
                    printf("(LH): Load half word from address 0x%08X into x[%d] = 0x%08X\n",
                           effective_address, ptr_rd, rd);
                    break;
                }
                default:
                    goto illegal;
            }
            break;
        }
        case 0x2: { // SLTI
            switch(opcode){
                case 0x13:{
                    rd = ((int32_t)rs1 < (int32_t)imm) ? 1u : 0u;
                    printf("(SLTI): %u = %u < %u\n",rd,rs1,imm);
                    break;
                }
                case 0x3: {
                // LW load word
                    // Calculate memory address
                    uint32_t effective_address = rs1 + imm;
                    uint32_t temp = effective_address % 4;
                    if(temp) {printf("Ignores that offset is not multiple of 4. ");}
                    uint32_t rd = get_memory(effective_address-temp);

                    // For debugging: print the result
                    printf("(LW): Loaded word from address 0x%08X into x[%d] = 0x%08X\n",
                           effective_address, ptr_rd, rd);
                    break; 
                }
            }
            break;
        }
        case 0x3: {
            switch(opcode){
                case 0x13:{
                    rd = (rs1 < imm) ? 1u : 0u;
                    printf("(SLTIU): %u = %u < %u\n",rd,rs1,imm);
                    break;
                }
                default:
                  goto illegal;
            }
            break;
        }
        case 0x4: { // XORI
            switch(opcode){
                case 0x13:{
                    rd = rs1 ^ imm;
                    printf("(XORI): %u = %u ^ %u\n",rd,rs1,imm);
                    break;
                }
                case 0x3: {
                    // lbu
                    // Calculate memory address
                    uint32_t effective_address = rs1 + imm;
                    uint32_t temp = effective_address % 4;

                    uint32_t word = get_memory(effective_address-temp);
                    uint32_t rd = (word >> (temp * 8)) & 0xFF;

                    // For debugging: print the result
                    printf("(LBU): Loaded unsigned byte from address 0x%08X into x[%d] = 0x%08X\n",
                           effective_address, ptr_rd, rd);
                    break;
                  }
                default:
                  goto illegal;
            }
            break;
        }
        case 0x5: { // SRLI / SRAI
            switch(opcode){
                case 0x13:{
                    uint32_t imm_hi = get_bits(imm, 11, 5);
                    switch (imm_hi) {
                        case 0x00: {
                            rd = rs1 >> (imm & 0x1F);
                            printf("(SRLI): %u = %u >> %u\n",rd,rs1,imm);
                            break;                          // SRLI
                        }
                        case 0x20: {
                            rd = (uint32_t)(((int32_t)rs1) >> (imm & 0x1F));
                            printf("(SRAI): %u = %u >> %u\n",rd,rs1,imm);
                            break;   // SRAI
                        }
                        default:   goto illegal;
                    }
            break;
                 }
                case 0x3: {
                    // LHU load half word unsigned
                    // Calculate memory address
                    uint32_t effective_address = rs1 + imm;
                    uint32_t temp = effective_address % 4;

                    uint32_t word = get_memory(effective_address-temp);
                    uint32_t rd = (word >> (temp * 8)) & 0xFFFF;

                    // For debugging: print the result
                    printf("(LHU): Loaded unsigned half word from address 0x%08X into x[%d] = 0x%08X\n",
                           effective_address, ptr_rd, rd);

                    break;
                }
                default:
                  goto illegal;
            }
            break;
        }
        case 0x6: { // ORI
            switch(opcode){
                case 0x13:{
                    rd = rs1 | imm;
                    printf("(ORI): %u = %u | %u\n",rd,rs1,imm);
                    break;
                }
                default:
                  goto illegal;
            }
            break;
        }
        case 0x7: { // ANDI
            switch(opcode){
                case 0x13:{
                    rd = rs1 & imm;
                    printf("(ANDI): %u = %u & %u\n",rd,rs1,imm);
                    break;
                }
                default:
                    goto illegal;
            }
            break;
        }
        default:
            goto illegal;
    }

    if (ptr_rd) set_register(ptr_rd, rd);   // keep x0 hardwired to 0
    return;

    illegal: {
        fprintf(stderr, "Illegal I-type encoding (opcode=0x%02X, funct3=0x%X)\n",
                (unsigned)opcode, (unsigned)funct3);
                  
        }
    }
