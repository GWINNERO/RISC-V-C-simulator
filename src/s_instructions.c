#include <stdio.h>
#include <stdint.h>
#include "instructions.h"
#include "memory.h"


/* ---------- S-type executor (fixed) ---------- */
void execute_s_type(uint32_t instr) {
    const uint32_t rs1    = get_rs1(instr);
    const uint32_t rs2    = get_rs2(instr);
    const uint32_t funct3 = get_funct3(instr);

    /* S-type immediate is split between bits [31:25] and [11:7] */
    const uint32_t imm_31_25 = get_bits(instr, 31, 25);
    const uint32_t imm_11_7  = get_bits(instr, 11, 7);
    const uint32_t imm12     = (imm_31_25 << 5) | imm_11_7;

    /* Sign-extend 12-bit immediate to 32-bit signed integer */
    const int32_t simm12 = (int32_t)((int32_t)(imm12 << 20) >> 20);

    const uint32_t base   = get_register(rs1);
    const uint32_t value  = get_register(rs2);
    const uint32_t addr   = base + (uint32_t)simm12;
    
    switch (funct3) {
        case 0x0: { // Store byte (SB)
            sb(addr, (uint32_t)(value & 0xFF));
            printf("(SB)\n");
            break;
        }
        case 0x1: { // Store half (SH)
            sh(addr, (uint32_t)(value & 0xFFFF));
            printf("(SH)\n");
            break;
        }
        case 0x2: { // Store word (SW)
            sw(addr, value);
            printf("(SW)\n");
            break;
        }
        default:
            goto illegal;
    }

    // S-type does NOT write to any register
    return;

illegal:
    fprintf(stderr, "Illegal S-type encoding (funct3=0x%X)\n",
            (unsigned)funct3);
}


// store the lowest 8 bits of value into memory
void sb(uint32_t addr, uint32_t value) {
    uint32_t index = addr / 4;
    memory[index] = (uint32_t)(value & 0xFF);  // keep only 8 bits
    printf("SB mem[%u] = 0x%02X\n", index, memory[index]);
}

// stores the lowest 16 bits of value into memory
void sh(uint32_t addr, uint32_t value) {
    uint32_t index = addr / 4;
    memory[index] = (uint32_t)(value & 0xFFFF);  // keep 16 bits
    printf("SH mem[%u] = 0x%04X\n", index, memory[index]);
}

// stores all 32 bits of value into memory
void sw(uint32_t addr, uint32_t value) {
    uint32_t index = addr / 4;
    memory[index] = value;
    printf("SW mem[%u] = 0x%08X\n", index, memory[index]);
}
