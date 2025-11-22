#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

// dispatcher
void dispatch_type(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F; // last 7 bits

    switch (opcode) {
        case 0x33: // R-type
            execute_r_type(instruction);
            break;

        case 0x03: case 0x0F: case 0x13: case 0x67: case 0x73: // I-type group
            execute_i_type(instruction);
            break;

        case 0x23: // S-type
            execute_s_type(instruction);
            break;

        case 0x63: // SB-type
            execute_sb_type(instruction);
            break;

        case 0x17: case 0x37: // U-type
            execute_u_type(instruction);
            break;

        case 0x6F: // UJ-type
            execute_uj_type(instruction);
            break;

        default:
            printf("Non existent opcode ERROR: 0x%02X\n", (unsigned)opcode);
            break;
    }
}


/* ----- bit helpers ----- */
static inline uint32_t get_bits(uint32_t x, int hi, int lo) {
    int w = hi - lo + 1;
    return (x >> lo) & ((w == 32) ? 0xFFFFFFFFu : ((1u << w) - 1u));
}
static inline uint32_t get_rd    (uint32_t inst){ return get_bits(inst, 11, 7);  }
static inline uint32_t get_funct3(uint32_t inst){ return get_bits(inst, 14, 12); }
static inline uint32_t get_rs1   (uint32_t inst){ return get_bits(inst, 19, 15); }
static inline uint32_t get_rs2   (uint32_t inst){ return get_bits(inst, 24, 20); }
static inline uint32_t get_funct7(uint32_t inst){ return get_bits(inst, 31, 25); }

/* simple global register file for demo; x0 must stay 0 */
//static uint32_t regs[32] = {0};

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
                case 0x00: rd = (uint32_t)((int32_t)rs1 + (int32_t)rs2); printf("ADD "); break; // ADD
                case 0x20: rd = (uint32_t)((int32_t)rs1 - (int32_t)rs2); printf("SUB "); break; // SUB
                default:   goto illegal;
            } break;
        }
        case 0x1: { // SLL
            if (funct7 != 0x00) goto illegal;
            rd = rs1 << (rs2 & 0x1F);
            printf("SLL ");
            break;
        }
        case 0x2: { // SLT (signed)
            if (funct7 != 0x00) goto illegal;
            rd = ((int32_t)rs1 < (int32_t)rs2) ? 1u : 0u;
            printf("SLT ");
            break;
        }
        case 0x3: { // SLTU (unsigned)
            if (funct7 != 0x00) goto illegal;
            rd = (rs1 < rs2) ? 1u : 0u;
            printf("SLTU ");
            break;
        }
        case 0x4: { // XOR
            if (funct7 != 0x00) goto illegal;
            rd = rs1 ^ rs2;
            printf("XOR ");
            break;
        }
        case 0x5: { // SRL / SRA
            switch (funct7) {
                case 0x00: rd = rs1 >> (rs2 & 0x1F);printf("SRL "); break;                         // SRL
                case 0x20: rd = (uint32_t)(((int32_t)rs1) >> (rs2 & 0x1F));printf("SRA "); break;  // SRA
                default:   goto illegal;
            } break;
        }
        case 0x6: { // OR
            if (funct7 != 0x00) goto illegal;
            rd = rs1 | rs2;
            printf("OR ");
            break;
        }
        case 0x7: { // AND
            if (funct7 != 0x00) goto illegal;
            rd = rs1 & rs2;
            printf("AND ");
            break;
        }
        default:
            goto illegal;
    }

    if (ptr_rd) set_register(ptr_rd, rd);   // keep x0 hardwired to 0
    printf("R type execute, set %u to %u\n", ptr_rd, rd);
    return;

    illegal:
        fprintf(stderr, "Illegal R-type encoding (funct7=0x%02X, funct3=0x%X)\n",
                (unsigned)funct7, (unsigned)funct3);
}

/* ---------- I-type executor (fixed) ---------- */
void execute_i_type(uint32_t instr) {
    const uint32_t ptr_rd     = get_rd(instr);
    const uint32_t ptr_rs1    = get_rs1(instr);
    const uint32_t imm    = get_bits(instr, 31, 20);
    const uint32_t funct3 = get_funct3(instr);
    const uint32_t funct7 = get_funct7(instr);

    const uint32_t rs1 = get_register(ptr_rs1); // a
    uint32_t rd = 0;

    switch (funct3) {
        case 0x0: { // ADDI
            rd = (uint32_t)((int32_t)rs1 + (int32_t)imm);
            break;
        }
        case 0x1: { // SLLI
            if (funct7 != 0x00) goto illegal;
            rd = rs1 << (imm & 0x1F);
            break;
        }
        case 0x2: { // SLTI
            rd = ((int32_t)rs1 < (int32_t)imm) ? 1u : 0u;
            break;
        }
        case 0x3: { // SLTIU
            rd = (rs1 < imm) ? 1u : 0u;
            break;
        }
        case 0x4: { // XORI
            rd = rs1 ^ imm;
            break;
        }
        case 0x5: { // SRLI / SRAI
            uint32_t imm_hi = get_bits(imm, 11, 5);
            switch (imm_hi) {
                case 0x00: rd = rs1 >> (imm & 0x1F); break;                          // SRLI
                case 0x20: rd = (uint32_t)(((int32_t)rs1) >> (imm & 0x1F)); break;   // SRAI
                default:   goto illegal;
            }
            break;
        }
        case 0x6: { // ORI
            rd = rs1 | imm;
            break;
        }
        case 0x7: { // ANDI
            rd = rs1 & imm;
            break;
        }
        default:
            goto illegal;
    }

    if (ptr_rd) set_register(ptr_rd, rd);   // keep x0 hardwired to 0
    printf("I type execute, set %u to %u\n", ptr_rd, rd);
    return;

    illegal:
        fprintf(stderr, "Illegal I-type encoding (funct7=0x%02X, funct3=0x%X)\n",
                (unsigned)funct7, (unsigned)funct3);
}
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



void execute_sb_type(uint32_t x){ (void)x; puts("SB-type stub"); }
void execute_u_type(uint32_t x){ (void)x; puts("U-type stub"); }
void execute_uj_type(uint32_t x){ (void)x; puts("UJ-type stub"); }

/* ---------- demo main ---------- */
/*
int main(void) {
    // Encode: ADD x3, x1, x2  => funct7=0x00 rs2=2 rs1=1 funct3=0 rd=3 opcode=0x33
    uint32_t add_x3_x1_x2 =
        (0x00u << 25) | (2u << 20) | (1u << 15) | (0x0u << 12) | (3u << 7) | 0x33u;

    regs[1] = 10;
    regs[2] = 3;

    dispatch_type(add_x3_x1_x2);
    printf("x3 = %u\n", regs[3]); // expected: 13

    return 0;
}
*/
