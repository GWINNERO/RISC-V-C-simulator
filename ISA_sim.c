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
#include <stdint.h>

#define PROGRAM_SIZE 3
#define REG_SIZE 4

int pc;
int reg[REG_SIZE];

int progr[PROGRAM_SIZE] = {
    0x00200093, // addi x1 x0 2
    0x00300113, // addi x2 x0 3
    0x002081b3  // add x3 x1 x2
};


void print_binary(uint32_t value) {
    // Array to hold the binary representation
    char binary_str[33]; // 32 bits + null terminator
    binary_str[32] = '\0'; // Null-terminate the string

    for (int i = 31; i >= 0; i--) {
        // Set the corresponding bit in the string
        binary_str[i] = (value & 1) ? '1' : '0';
        value >>= 1; // Shift the value right by 1 bit
    }

    printf("The value in binary is: %s\n", binary_str);
}

void execute_r_type(uint32_t instr) {
    uint32_t opcode = instr & 0x7F;
    printf("The value is: %x \n", opcode);
    print_binary(opcode);
    uint32_t rd     = (instr >> 7) & 0x1F;
    uint32_t funct3 = (instr >> 12) & 0x7;
    uint32_t rs1    = (instr >> 15) & 0x1F;
    uint32_t rs2    = (instr >> 20) & 0x1F;
    uint32_t funct7 = (instr >> 25);

    switch (funct3) {
        case 0x0:
        switch(funct7) {
            case 0x0:
                add(rd, rs1, rs2);
                break;
            }
        break;
    }
}


void add(uint32_t rd, uint32_t rs1, uint32_t rs2){
    printf("rd: ");
    print_binary(rd);
    printf("\n rs1:");
    print_binary(rs1);
    printf("\nrs2:");
    print_binary(rs2);
}


int main() {
    printf("Hello RISC-V World!\n");

    execute_r_type(0x00FF00F3);

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
