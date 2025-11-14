// memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define MAX_INSTRUCTIONS 262144   // enough for 1 MB / 4 bytes

int load_file(const char *filename, uint32_t instr_mem[]);

#endif // MEMORY_H