#include <stdio.h>
#include <stdint.h>

int main(void) {
    FILE *f_loop = fopen("results/register_dump_loop.bin", "rb");
    FILE *f_loop_exp = fopen("test_result_files/loop.res", "rb");
    FILE *f_rec = fopen("results/register_dump_recursive.bin", "rb");
    FILE *f_rec_exp = fopen("test_result_files/recursive.res", "rb");

    uint32_t val;
    printf("Loop test:\n");
    for (int i = 0; i < 8; i++) {
        fread(&val, 4, 1, f_loop);
        printf("  x[%d] = 0x%08X", i, val);
        fread(&val, 4, 1, f_loop_exp);
        printf(" (expected 0x%08X)\n", val);
    }

    printf("\nRecursive test:\n");
    for (int i = 0; i < 8; i++) {
        fread(&val, 4, 1, f_rec);
        printf("  x[%d] = 0x%08X", i, val);
        fread(&val, 4, 1, f_rec_exp);
        printf(" (expected 0x%08X)\n", val);
    }

    fclose(f_loop);
    fclose(f_loop_exp);
    fclose(f_rec);
    fclose(f_rec_exp);

    return 0;
}
