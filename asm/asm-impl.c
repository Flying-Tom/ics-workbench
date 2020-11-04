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
    /*asm(
        "xor %%esi,%%esi;"
        "mov %%rbx,(%%rdi);"
        "mov %%rbp,%%rax;"
        "xor %fs:0x30,%%rax;"
        "rol $0x11,%%rax;"
        "mov %%rax,0x8(%%rdi);"
        "mov %%r12,0x10(%%rdi);"
        "mov %%r13,0x18(%%rdi);"
        "mov %%r14,0x20(%%rdi);"
        "mov %%r15,0x28(%%rdi);"
        "lea 0x8(%%rsp),%%rdx;"
        "xor %fs:0x30,%%rdx;"
        "rol $0x11,%%rdx;"
        "mov %%rdx,0x30(%rdi);"
        "mov (%%rsp),%%rax;"
        "xor %fs:0x30,%%rax;"
        "rol $0x11,%%rax;"
        "mov %%rax,0x38(%%rdi);"
        "push %%rbx;"
        "mov %%rdi,%%rbx;"
        "test %%esi,%%esi;"

    );
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
        "push   %%rcx;"
        "mov    %%rcx,(%%rdi);"
        "mov    %%rdx,0(%%rdi);"
        "mov    %%rbx,8(%%rdi);"
        "mov    %%rbp,16(%%rdi);"
        "mov    %%rbx,24(%%rdi);"
        "mov    %%r12,32(%%rdi);"
        "mov    %%r13,40(%%rdi);"
        "mov    %%r14,48(%%rdi);"
        "mov    %%r15,56(%%rdi);"
        "mov    %rdi,%0;"
        "xor    %%rax,%%rax;"
        "jmp    *%%rcx"
        : "=r"(temp)
        : "r"(env)
        );
    return temp;
}

void asm_longjmp(asm_jmp_buf env, int val)
{
    longjmp(env, val);
    /*
    asm(

        "mov    0x30(%%rdi),%%r8;"
        "mov    0x8(%%rdi),%r9;"
        "mov    0x38(%%rdi),%%rdx;"
        "ror    $0x11,%%r8;"
        "xor    %fs:0x30,%%r8;"
        "ror    $0x11,%%r9;"
        "xor    %fs:0x30,%%r9;"
        "ror    $0x11,%%rdx;"
        "xor    %fs:0x30,%%rbx;"
        "mov    (%%rdi),%rbx;"
        "mov    0x10(%%rdi),%%r12;"
        "mov    0x18(%%rdi),%%r13;"
        "mov    0x20(%%rdi),%%r14;"
        "mov    0x28(%%rdi),%%r15;"
        "mov    %%esi,%%eax;"
        "mov    %%r8,%%rsp;"
        "mov    %%r9,%%rbp;"
        "jmpq    *%%rdx;"
        //: "+r"(temp)
        ///: "r"(env)
        );*/
}
