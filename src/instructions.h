// instructions.h

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdio.h>
#include <stdint.h>

//void add(uint32_t rd, uint32_t rs1, uint32_t rs2);

// Forward declarations
void set_register(uint32_t reg, uint32_t value);
uint32_t get_register(uint32_t reg);
void print_binary(uint32_t value);
void set_pc(uint32_t value);
uint32_t get_pc();

//void get_bits(uint32 x, int hi, int lo);

void dispatch_type(uint32_t instruction);

void execute_r_type(uint32_t instr);
void execute_i_type(uint32_t instr);
void execute_s_type(uint32_t instr);
void execute_sb_type(uint32_t instr);
void execute_u_type(uint32_t instr);
void execute_uj_type(uint32_t instr);

uint32_t get_bits(uint32_t x, int hi, int lo);
uint32_t get_rd(uint32_t inst);
uint32_t get_funct3(uint32_t inst);
uint32_t get_rs1(uint32_t inst);
uint32_t get_rs2(uint32_t inst);
uint32_t get_funct7(uint32_t inst);

#endif // INSTRUCTIONS_H
