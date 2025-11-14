#include <stdio.h>
#include <stdint.h>

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

// example files
void execute_r_type(uint32_t instruction) {
    (void)instruction;
    printf("Called R_type function\n");
}

void execute_i_type(uint32_t instruction) {
    (void)instruction;
    printf("Called I_type function\n");
}

void execute_s_type(uint32_t instruction) {
    (void)instruction;
    printf("Called S_type function\n");
}

void execute_sb_type(uint32_t instruction) {
    (void)instruction;
    printf("Called SB_type function\n");
}

void execute_u_type(uint32_t instruction) {
    (void)instruction;
    printf("Called U_type function\n");
}

void execute_uj_type(uint32_t instruction) {
    (void)instruction;
    printf("Called UJ_type function\n");
}

// main
main() {
    uint32_t instruction = 0b000000000000000000000000000010011; // example R-type
    dispatch_type(instruction);
    return 0;
}