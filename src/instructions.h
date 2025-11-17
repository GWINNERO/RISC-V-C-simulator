// instructions.h
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h>
#include <stdint.h>

// Declare your functions here
void print_binary(uint32_t value);
void add(uint32_t rd, uint32_t rs1, uint32_t rs2);
void execute_r_type(uint32_t instr);
void set_register(uint32_t reg, uint32_t value);
uint32_t get_register(uint32_t reg);

#endif // INSTRUCTIONS_H
