#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

/* ---------- S-type executor (fixed) ---------- */
void execute_s_type(uint32_t instr) {
    const uint32_t rs1    = get_rs1(instr);
    const uint32_t rs2    = get_rs2(instr);
    const uint32_t funct3 = get_funct3(instr);

    // S-type immediate is split between bits 31-25 and 11-7
    const uint32_t imm_31_25 = get_bits(instr, 31, 25);
    const uint32_t imm_11_7  = get_bits(instr, 11, 7);
    const uint32_t imm12     = (imm_31_25 << 5) | imm_11_7;
    const int32_t  simm12    = sign_extend(imm12, 12);

    const uint32_t base   = get_register(rs1);      // or regs[rs1] if we don’t use get_register
    const uint32_t value  = get_register(rs2);
    const uint32_t addr   = base + (uint32_t)simm12;

    switch (funct3) {
        case 0x0: { // Store byte (SB)
            store_byte(addr, (uint8_t)(value & 0xFF));
            break;
        }
        case 0x1: { // Store half (SH)
            store_half(addr, (uint16_t)(value & 0xFFFF));
            break;
        }
        case 0x2: { // Store word (SW)
            store_word(addr, value);
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