#include <stdio.h>
#include <stdint.h>
#include "instructions.h"

// Fake register + PC storage for testing
uint32_t regs[32];
uint32_t pc;

uint32_t get_pc() { return pc; }
void set_pc(uint32_t newpc) { pc = newpc; }

void set_register(uint32_t r, uint32_t v) { regs[r] = v; }
uint32_t get_register(uint32_t r) { return regs[r]; }

// Paste your final jal() here
void jal(uint32_t instruction);

uint32_t get_bits(uint32_t x, int hi, int lo) {
    uint32_t mask = (1u << (hi - lo + 1)) - 1;
    return (x >> lo) & mask;
}

int main() {
    // JAL x1, +8   (simple forward jump)
    uint32_t instr = 0b00011000010000000000000011101111;
    pc = 0; // start PC

    printf("Before: PC=%u\n", pc);
    jal(instr);
    printf("After:  PC=%u, x1=%u\n", pc, regs[1]);

    return 0;
}
