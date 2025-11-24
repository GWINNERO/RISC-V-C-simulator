#include <stdio.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    
    FILE *f = fopen(argv[1], "rb");
    if (!f) return 1;
    
    uint32_t word;
    int i = 0;
    while (fread(&word, sizeof(uint32_t), 1, f)) {
        printf("x[%d]: 0x%08X\n", i++, word);
    }
    fclose(f);
    return 0;
}
