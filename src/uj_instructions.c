#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

void jal(uint32_t instruction){
    uint32_t rd = get_rd(instruction);
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
    // execute_instruction() pre-increments PC by 4 before dispatch.
    // get_pc() therefore returns (instruction_address + 4).
    // The JAL target should be instruction_address + imm, so subtract 4.
    set_pc((get_pc() - 4) + (int32_t)imm);

    printf("(JAL) rd=x%u, imm=%d, return=0x%08X, new_pc=0x%08X\n",
       rd, (int32_t)imm, return_address, get_pc());
}

void execute_uj_type(uint32_t instr){
    uint32_t opcode = instr & 0x7F;
    if(opcode == 0b1101111){
        jal(instr);
    }
    else
    {
        printf("This UJ-type instruction is not implemented.\n");
    }
}
