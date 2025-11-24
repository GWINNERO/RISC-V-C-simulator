/**
 * RISC-V Instruction Set C Simulator
 * <p>
 * u type instructions 
 *
 * @author shikoubox
 * @author GWINNERO
 * @author ARTURBARCIJ
 */
#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

void execute_u_type(uint32_t instr) {
    uint32_t opcode = instr & 0x7F;
    uint32_t ptr_rd     = get_rd(instr);
    uint32_t imm    = get_bits(instr, 31, 12);

    uint32_t rd = 0;

    switch(opcode) {
        case 0x0:
            break;
        case 0b0110111: {
            rd = imm << 12;
            printf("(LUI): (rd)%u = (imm)%u\n", ptr_rd, rd);
            break;
            }
        case 0b0010111: {
            // AUIPC: rd = PC (instruction address) + (imm << 12)
            uint32_t pc_instr = get_pc() - 4; // adjust for pre-incremented PC
            rd = (imm << 12) + pc_instr;
            printf("(AUIPC) x[%u]=0x%08X (pc=0x%08X imm=0x%X)\n", ptr_rd, rd, pc_instr, imm);
            break;
        }
        default: {
            printf("no such opcode. \n");
            break;
        }
    }

    if (ptr_rd) set_register(ptr_rd, rd);
    return;

}

