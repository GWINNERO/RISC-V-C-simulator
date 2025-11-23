#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

void print_binary(uint32_t value) {
    char binary_str[33]; 
    binary_str[32] = '\0'; 

    for (int i = 31; i >= 0; i--) {
        // set corresponding bit in string
        binary_str[i] = (value & 1) ? '1' : '0';
        value >>= 1;
    }

    printf("%s", binary_str);
}

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
            printf("s-type: ");
            execute_s_type(instruction);
            printf("s-type: ");
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
inline uint32_t get_bits(uint32_t x, int hi, int lo) {
    int w = hi - lo + 1;
    return (x >> lo) & ((w == 32) ? 0xFFFFFFFFu : ((1u << w) - 1u));
}
inline uint32_t get_rd    (uint32_t inst){ return get_bits(inst, 11, 7);  }
inline uint32_t get_funct3(uint32_t inst){ return get_bits(inst, 14, 12); }
inline uint32_t get_rs1   (uint32_t inst){ return get_bits(inst, 19, 15); }
inline uint32_t get_rs2   (uint32_t inst){ return get_bits(inst, 24, 20); }
inline uint32_t get_funct7(uint32_t inst){ return get_bits(inst, 31, 25); }
