#include <stdio.h>
#include <stdint.h>

/* ---------- forward declarations ---------- */
void execute_r_type(uint32_t instr);
void execute_i_type(uint32_t instr);
void execute_s_type(uint32_t instr);
void execute_sb_type(uint32_t instr);
void execute_u_type(uint32_t instr);
void execute_uj_type(uint32_t instr);

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
static uint32_t regs[32] = {0};

/* ---------- R-type executor (fixed) ---------- */
void execute_r_type(uint32_t instr) {
    const uint32_t rd     = get_rd(instr);
    const uint32_t rs1    = get_rs1(instr);
    const uint32_t rs2    = get_rs2(instr);
    const uint32_t funct3 = get_funct3(instr);
    const uint32_t funct7 = get_funct7(instr);

    const uint32_t a = regs[rs1];
    const uint32_t b = regs[rs2];
    uint32_t res = 0;

    switch (funct3) {
        case 0x0: { // ADD / SUB
            switch (funct7) {
                case 0x00: res = (uint32_t)((int32_t)a + (int32_t)b); break; // ADD
                case 0x20: res = (uint32_t)((int32_t)a - (int32_t)b); break; // SUB
                default:   goto illegal;
            } break;
        }
        case 0x1: { // SLL
            if (funct7 != 0x00) goto illegal;
            res = a << (b & 0x1F);
            break;
        }
        case 0x2: { // SLT (signed)
            if (funct7 != 0x00) goto illegal;
            res = ((int32_t)a < (int32_t)b) ? 1u : 0u;
            break;
        }
        case 0x3: { // SLTU (unsigned)
            if (funct7 != 0x00) goto illegal;
            res = (a < b) ? 1u : 0u;
            break;
        }
        case 0x4: { // XOR
            if (funct7 != 0x00) goto illegal;
            res = a ^ b;
            break;
        }
        case 0x5: { // SRL / SRA
            switch (funct7) {
                case 0x00: res = a >> (b & 0x1F); break;                         // SRL
                case 0x20: res = (uint32_t)(((int32_t)a) >> (b & 0x1F)); break;  // SRA
                default:   goto illegal;
            } break;
        }
        case 0x6: { // OR
            if (funct7 != 0x00) goto illegal;
            res = a | b;
            break;
        }
        case 0x7: { // AND
            if (funct7 != 0x00) goto illegal;
            res = a & b;
            break;
        }
        default:
            goto illegal;
    }

    if (rd) regs[rd] = res;   // keep x0 hardwired to 0
    regs[0] = 0;
    return;

    illegal:
        fprintf(stderr, "Illegal R-type encoding (funct7=0x%02X, funct3=0x%X)\n",
                (unsigned)funct7, (unsigned)funct3);
        regs[0] = 0;
}

void execute_I_type(uint32_t instr) {
    const uint32_t rd    = get_rd(instr);
    const uint32_t rs1   = get_rs1(instr);
    const uint32_t imm  = get_bits(instr, 31, 20);
    const uint32_t funct3 = get_funct3(instr);
    const uint32_t funct7 = get_funct7(instr);

    const uint32_t a = regs[rs1];
    const uint32_t imm[11:0] = imm;
    uint32_t res = 0;

    switch (funct3)
    {
    case 0x0 : // ADDI:
        res = a + imm;
        break;
    case 0x1: // SLLI:
        switch (funct7)
        {
        case (imm[5:11] = 0x00) : // SLLI::
            res = a << (imm[0:4] & 0x1F);
            break;
        default:
            break;
        }
        break;
    case 0x2 : // SLTI:
        res = ((int32_t)a < (int32_t)imm) ? 1u : 0u;
        break;
    case 0x3 : // SLTIU:
        res = (a < imm) ? 1u : 0u;
        break; 
    case 0x4 : // XORI:
        res = a ^ imm;
        break;
    case 0x5 : // SLRI:
        switch (funct7)
        {
        case (imm[5:11] = 0x00) : // SRLI::
            res = a >> (imm[0:4] & 0x1F);
            break;
        case (imm[5:11] = 0x20) : // SRAI::
            res = (uint32_t)(((int32_t)a) >> (imm[0:4] & 0x1F));
            break;
        default:
            break;
        }
    case 0x6 : // ORI:
        res = a | imm;
        break;
    case 0x7 : // ANDI:
        res = a & imm;
        break;
    default:
        break;
    }
}
void execute_S_type(uint32_t instr) {
    (void)instr;
    puts("S-type executor called");
}

/* ---------- minimal stubs so it links ---------- */
void execute_i_type(uint32_t x){ (void)x; puts("I-type stub"); }
void execute_s_type(uint32_t x){ (void)x; puts("S-type stub"); }
void execute_sb_type(uint32_t x){ (void)x; puts("SB-type stub"); }
void execute_u_type(uint32_t x){ (void)x; puts("U-type stub"); }
void execute_uj_type(uint32_t x){ (void)x; puts("UJ-type stub"); }

/* ---------- demo main ---------- */
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