// memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdint.h>

#define MAX_MEMORY 262144   // 1 MB / 4 bytes = 262144 instructions

// Load a .bin file with 32-bit instructions into memory[]
// Returns: number of 32-bit instructions loaded, or (uint32_t)-1 (max value) if an error occurs

uint32_t load_file(const char *filename, uint32_t memory[]);

// Dump registers to a binary file and print them in binary format to console
void bin_dump_registers(const uint32_t register_address[], int REG_COUNT);
#endif // MEMORY_H
