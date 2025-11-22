#include <stdio.h>
#include <stdint.h>
#include "instructions.h"



void jal(uint32_t instruction){
    uint32_t rd = get_bits(instruction, 11, 7);
    
    uint32_t imm20 = get_bits(instruction, 31, 31);
    uint32_t imm10_1 = get_bits(instruction, 30, 21);
    uint32_t imm11 = get_bits(instruction, 20, 20);
    uint32_t imm19_12 = get_bits(instruction, 19, 12);
    uint32_t imm = (imm20 << 20) | (imm19_12 << 12) | (imm11 << 11) | (imm10_1 << 1);
    
    if (imm20) {
        imm |= 0xFFE00000; // Sign-extend negative immediate
    }

    uint32_t return_address = get_pc();
    set_register(rd, return_address);
    set_pc(get_pc() + imm);

printf("JAL rd=x%u, imm=%d, return=0x%08X, new_pc=0x%08X\n",
       rd, (int32_t)imm, return_address, get_pc());

}