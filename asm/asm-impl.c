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
        "movl $0x0,%%ecx;"
        "movl $0x0,%0;"
        ".L1:"
        "movq %1,%%rdx;"
        "shrq %%cl,%%rdx;"
        "andl $0x1,%%edx;"
        "addl %%edx,%0;"
        "addl $0x1,%%ecx;"
        "cmpl $0x40,%%ecx;"
        "jne .L1;"
        : "+r"(s)
        : "r"(x));
    return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n)
{
    //return memcpy(dest, src, n);
    /*char *dp = dest;
    const char *sp = src;
    while (n--)
        *dp++ = *sp++;*/
    asm(
        "movq   %0,%%rax;"
        "test   %2,%2;"
        "je     .L2"
        "mov    $0x0,%%ecx;"
        ".L1:"
        "movzbl (%1,%%rcx,1),%%r8d;"
        "mov    %%r8b,(%%rax,%%rcx,1);"
        "add    $0x1,%%rcx;"
        "cmp    %2,%%rcx;"
        "jne    .L1;"
        ".L2:"
        "retq;"
        : "+r"(dest)
        : "r"(src), "r"(n));
    return dest;
}

int asm_setjmp(asm_jmp_buf env)
{
    return setjmp(env);
}

void asm_longjmp(asm_jmp_buf env, int val)
{
    longjmp(env, val);
}
