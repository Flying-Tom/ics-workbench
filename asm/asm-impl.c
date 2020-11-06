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
    //return setjmp(env);
    /*
    asm(
        ".plt:"
        "pushq  0x2fe2(%%rip);"
        "jmpq   *0x2fe4(%%rip);"
        "nopl   0x0(%%rax)"
        "_setjmp:"
        "jmpq *0x2fe2(%%rip);"
        "pushq $0x0;"
        "jmpq   .plt;"
        "longjmp:"
        "jmpq   *0x2fd2(%%rip);"
        "pushq  $0x2;"
        "jmpq   .plt;"
        );
    asm(
        "subq    $0x8,%%rsp;"
        "callq  _setjmp;"
        "addq    $0x8,%%rsp;"
        "retq;"
        );*/

    int temp = 0;
    asm(
        "mov    %1,%%rdi;"
        "mov    %%rcx,(%%rdi);"
        "mov    %%rdx,0(%%rdi);"
        "mov    %%rsp,8(%%rdi);"
        "mov    %%rbp,16(%%rdi);"
        "mov    %%rbx,24(%%rdi);"
        "mov    %%r12,32(%%rdi);"
        "mov    %%r13,40(%%rdi);"
        "mov    %%r14,48(%%rdi);"
        "mov    %%r15,56(%%rdi);"
        "pop    64(%%rdi);"
        "push    64(%%rdi);"
        "xor    %0,%0;"
        "lea    (%0),%0;"
        : "=r"(temp)
        : "r"(env)
        );
    return temp;
}

void asm_longjmp(asm_jmp_buf env, int val)
{
    //longjmp(env, val);
    asm(
        "mov    %1,%%rdi;"
        "mov    (%%rdi),%%rcx;"
        "mov    0(%%rdi),%%rdx;"
        "mov    8(%%rdi),%%rsp;"
        "mov    16(%%rdi),%%rbp;"
        "mov    24(%%rdi),%%rbx;"
        "mov    32(%%rdi),%%r12;"
        "mov    40(%%rdi),%%r13;"
        "mov    48(%%rdi),%%r14;"
        "mov    56(%%rdi),%%r15;"
        "mov    72(%%rdi),%0;"
        "jmpq    64(%%rdi);"
        : "+r"(val)
        : "r"(env)
        );
}
