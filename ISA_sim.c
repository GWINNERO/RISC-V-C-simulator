/**
 * RISC-V Instruction Set C Simulator
 * <p>
 * Example of basic C simulator
 *
 * @author shikoubox
 * @author GWINNERO
 * @author ARTURBARCIJ
 */

#include <stdio.h>

#define PROGRAM_SIZE 3
#define REG_SIZE 4

int pc;
int reg[REG_SIZE];

int progr[PROGRAM_SIZE] = {
    0x00200093, // addi x1 x0 2
    0x00300113, // addi x2 x0 3
    0x002081b3  // add x3 x1 x2
};

void r_type(uint32_t instr) {

    int opcode = instr & 0x7F;
    print(opcode);
    //int rd = uint32[11:7];
    //int funct3 = uint32[14:12];
    //int rs1 = uint32[19:15];
    //int rs2 = uint32[24:20];
    //int funct7 = uint32[31:25];

    printf("Value: %" PRIu32 "\n", value);
}


int main() {
    printf("Hello RISC-V World!\n");

    pc = 0;

    while (1) {
        int instr = progr[pc >> 2];
        int opcode = instr & 0x7F;
        int rd = (instr >> 7) & 0x1F;
        int rs1 = (instr >> 15) & 0x1F;
        int imm = (instr >> 20);

        switch (opcode) {
            case 0x13:
                reg[rd] = reg[rs1] + imm;
                break;
            default:
                printf("Opcode %02x not yet implemented\n", opcode);
                break;
        }

        pc += 4; // One instruction is four bytes

        if ((pc >> 2) >= PROGRAM_SIZE) {
            break;
        }

        for (int i = 0; i < REG_SIZE; ++i) {
            printf("%d ", reg[i]);
        }
        printf("\n");
    }

    printf("Program exit\n");
    return 0;
}
