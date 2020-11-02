#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b)
{
    int64_t temp;
    asm(
        "addq %1, %2;"
        "movq %2, %0;"
        : "=r"(temp)
        : "r"(a), "r"(b));
    return temp;
}

int asm_popcnt(uint64_t x)
{
    int s = 0;
    /*
    for (int i = 0; i < 64; i++)
    {
        if ((x >> i) & 1)
            s++;
    }*/
    asm(
        ".L1"
        "mov $0x0,%ecx;"
        "mov $0x0,%eax;"
        "mov %rdi,%rdx;"
        "shr %cl,%rdx;"
        "and $0x1,%edx;"
        "add %edx,%eax;"
        "add $0x1,%ecx;"
        "cmp $0x40,%ecx;");
    return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n)
{
    return memcpy(dest, src, n);
}

int asm_setjmp(asm_jmp_buf env)
{
    return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val)
{
    longjmp(env, val);
}
