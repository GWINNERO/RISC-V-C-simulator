// memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdint.h>

#define MAX_MEMORY 0xF4240   // 1 MB / 4 bytes = 262144 instructions

// Load a .bin file with 32-bit instructions into memory[]
// Returns: number of 32-bit instructions loaded, or (uint32_t)-1 (max value) if an error occurs

uint32_t load_file(const char *filename, uint32_t memory[]);

// Dump registers to a binary file and print them in binary format to console
void bin_dump_registers(const uint32_t register_address[], int REG_COUNT, const char *input_filename);


extern uint32_t memory[MAX_MEMORY];    // Instruction memory start at 0
                                // Stack memory start at 1 MB (262144)
extern int memory_full;


uint32_t get_memory(uint32_t address);
void set_memory(uint32_t address, uint32_t value);

void write_byte(uint32_t addr, uint8_t value);
uint32_t read_byte(uint32_t addr);

#endif // MEMORY_H
