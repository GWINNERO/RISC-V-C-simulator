#include <stdio.h>
#include <stdint.h>
#include "instructions.h"
#include "memory.h"

/* ---------- I-type executor (fixed) ---------- */
void execute_i_type(uint32_t instr) {
    const uint32_t opcode     = instr & 0x7F;
    const uint32_t ptr_rd     = get_rd(instr);
    const uint32_t ptr_rs1    = get_rs1(instr);
    const uint32_t imm    = (instr >> 20) & 0xFFF;
    const uint32_t funct3 = get_funct3(instr);

    int32_t sign_imm = (imm & 0x800) ?(imm | 0xFFFFF000) :(imm & 0xFFF);
    const uint32_t rs1 = get_register(ptr_rs1); // a
    uint32_t rd = 0;

    switch (funct3) {
        case 0x0: { // ADDI
            switch(opcode){
                case 0x13: {

                    rd = (int32_t)(rs1 + sign_imm);
                    printf("(ADDI): [x%u]0x%X = [x%u]0x%X + %d\n",ptr_rd,rd,ptr_rs1,rs1,sign_imm);
                    break;
                }
                case 0x03: { // LB
                    // Calculate memory address (signed 12-bit immediate)
                    uint32_t addr = rs1 + (uint32_t)sign_imm;

                    uint32_t b = read_byte(addr);

                    // Sign-extend the byte to 32-bits
                    if (b & 0x80) { 
                       rd = b | 0xFFFFFF00;
                    }
                    else{rd = (int32_t)b;}

                    // For debugging: print the result
                    printf("(LB) Loaded byte from memory[%u] 0x%02X into x[%d] = (0x%08X)\n",
                       addr, b, ptr_rd, rd);


                    break;
                }
                case 0x67: {
                    // JALR: write return address (PC+4) into destination register,
                    // then set PC = (rs1 + imm) & ~1. Use ptr_rd (destination index).
                    {
                        int32_t offset = sign_imm;
                        uint32_t target = (uint32_t)((int32_t)rs1 + offset) & ~1u;
                        set_register(ptr_rd, get_pc());
                        set_pc(target);
                        printf("(JALR) rd=x%u rs1=x%u imm=%d new_pc=0x%08X\n",
                               ptr_rd, ptr_rs1, offset, target);
                    }
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
                case 0x3: { // LH
                    // LH load half word
                    uint32_t addr = rs1 + (uint32_t)sign_imm;

                    uint8_t b0 = read_byte(addr);
                    uint8_t b1 = read_byte(addr + 1);

                    uint32_t half = (b1 << 8) | b0;

                    rd = (half & 0x8000) ? (int32_t)(half | 0xFFFF0000)
                                                 : (int32_t)half;

                    printf("(LH) Load half word from memory[%u:%u] 0x%04X into x[%d] = 0x%08X\n",
                           addr, addr+1, half, ptr_rd, rd);

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
                    rd = (int32_t)rs1 < sign_imm ? 1u : 0u;
                    printf("(SLTI): [x%u]%u = [x%u]%d < %d\n",ptr_rd,rd,ptr_rs1,rs1,sign_imm);
                    break;
                }
                case 0x3: {
                // LW load word
                    uint32_t addr = rs1 + (uint32_t)sign_imm;

                    uint8_t b0 = read_byte(addr);
                    uint8_t b1 = read_byte(addr + 1);
                    uint8_t b2 = read_byte(addr + 2);
                    uint8_t b3 = read_byte(addr + 3);

                    rd =
                        ((uint32_t)b3 << 24) |
                        ((uint32_t)b2 << 16) |
                        ((uint32_t)b1 <<  8) |
                        ((uint32_t)b0 <<  0);

                    printf("(LW): Loaded word from memory[%u:%u] 0x%08X into x[%d] = 0x%08X\n",
                           addr, addr+3, rd, ptr_rd, rd);

                    break;
                }
            }
            break;
        }
        case 0x3: {
            switch(opcode){
                case 0x13:{
                    rd = (rs1 < imm) ? 1u : 0u;
                    printf("(SLTIU): [x%u]%u = [x%u]%u < %u\n",ptr_rd,rd,ptr_rs1,rs1,imm);
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
                    // LBU 
                    uint32_t addr = rs1 + (uint32_t)sign_imm;

                    uint8_t b = read_byte(addr);
                    rd = b;

                    printf("(LBU): Loaded unsigned byte from memory[%u] 0x%02X into x[%d] = 0x%08X\n",
                           addr, b, ptr_rd, rd);
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
                    uint32_t addr = rs1 + (uint32_t)sign_imm;

                    uint8_t b0 = read_byte(addr);
                    uint8_t b1 = read_byte(addr + 1);

                    rd = (b1 << 8) | b0;

                    printf("(LHU) Loaded unsigned half word from memory[%u:%u] 0x%04X into x[%d] =  (0x%08X)\n",
                           addr, addr+1, rd, ptr_rd, rd);
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
