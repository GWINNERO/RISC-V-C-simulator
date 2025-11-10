#include <stdio.h>
#include <stdint.h>
#include <string.h>

void dispatch_type(uint32_t instruction) {
    uint32_t opcode =  instruction & 0x7F;
    switch (opcode) {
        // R-type instructions
        case 0x33:
            execute_r_type(instruction);
            break;
        // I-type instructions
        case 0x03:
        case 0x0F:
        case 0x13:
        case 0x67:
        case 0x73:
            execute_i_type(instruction);
            break;
        // S-type instructions
        case 0x23:
            execute_s_type(instruction);
            break;
        // SB-type instructions
        case 0x63:
            execute_sb_type(instruction);
            break;
        // U-type instructions
        case 0x17:
        case 0x37:
            execute_u_type(instruction);
            break;
        // UJ-type instructions
        case 0x6F:
            execute_uj_type(instruction);
            break;
        default:
            printf("Non existant opcode ERROR:", opcode);
            break;
    }
}

void execute_r_type(instruction) {
    printf("Called R_type function\n");
}

void execute_i_type(instruction) {
    printf("Called I_type function\n");
}

void execute_s_type(instruction) {
    printf("Called S_type function\n");
}

void execute_sb_type(instruction) {
    printf("Called SB_type function\n");
}

void execute_u_type(instruction) {
    printf("Called U_type function\n");
}

void execute_uj_type(instruction) {
    printf("Called UJ_type function\n");
}




int main() {
    uint32_t instruction = 0b00001000000000000000000010110011; // example instruction
    dispatch_type(instruction);
    return 0;
}