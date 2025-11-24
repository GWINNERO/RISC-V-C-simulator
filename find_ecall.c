#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    
    FILE *f = fopen(argv[1], "rb");
    if (!f) return 1;
    
    uint32_t word;
    uint32_t addr = 0;
    while (fread(&word, sizeof(uint32_t), 1, f)) {
        if (word == 0x73) {
            printf("Found ECALL at address 0x%04X\n", addr);
        }
        addr += 4;
    }
    fclose(f);
    return 0;
}
