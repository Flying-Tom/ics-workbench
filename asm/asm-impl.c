#include "asm.h"
//#include <string.h>

int64_t asm_add(int64_t a, int64_t b)
{
    int64_t temp;
    asm(
        "addq %1, %2;"
        "movq %2, %0;"
        : "=r"(temp)
        : "r"(a), "r"(b)

    );
    return temp;
}

int asm_popcnt(uint64_t x)
{
    int s = 0;
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
        : "r"(x)
        : "ecx", "rdx", "edx"

    );
    return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n)
{
    asm(
        "test   %2,%2;"
        "je     .L2;"
        "mov    $0x0,%%ecx;"
        ".L3:"
        "movzbl (%1,%%rcx,1),%%r8d;"
        "mov    %%r8b,(%0,%%rcx,1);"
        "add    $0x1,%%rcx;"
        "cmp    %2,%%rcx;"
        "jne    .L3;"
        ".L2:"
        : "+r"(dest)
        : "r"(src), "r"(n)
        : "ecx", "rcx", "rcx"

    );
    return dest;
}

int asm_setjmp(asm_jmp_buf env)
{
    int64_t temp = 0;
    asm(
        "mov    %%rbx,(%1);"
        "mov    %%rbp,8(%1);"
        "mov    %%rsp,16(%1);"
        "mov    %%r12,24(%1);"
        "mov    %%r13,32(%1);"
        "mov    %%r14,40(%1);"
        "mov    %%r15,48(%1);"
        "mov    (%%rsp),%%r15;"
        "mov    %%r15,56(%1);"
        "xor    %0,%0;"
        : "=r"(temp)
        : "r"(env)
        :

    );
    return temp;
}

volatile void asm_longjmp(asm_jmp_buf env, int val)
{
    asm(
        "mov    (%%rdi),%%rbx;"
        "mov    8(%%rdi),%%rbp;"
        "mov    16(%%rdi),%%rsp;"
        "mov    24(%%rdi),%%r12;"
        "mov    32(%%rdi),%%r13;"
        "mov    40(%%rdi),%%r14;"
        "mov    48(%%rdi),%%r15;"
        "pop    %%rax;"
        "mov    %%rsi,%%rax;"
        "jmp    *56(%%rdi);"
        :
        
        

    );
}
