#include <stdint.h>
#include <stddef.h>

// TODO: replace with your definition
//#include <setjmp.h> // remove this
typedef long int my_jmp_buf[8];
struct  my__jmp_buf_tag
{
    my_jmp_buf __jmp_buf;
};
typedef struct my__jmp_buf_tag jmp_buf[1];
#define asm_jmp_buf jmp_buf
asm_jmp_buf buf;

int64_t asm_add(int64_t a, int64_t b);
int     asm_popcnt(uint64_t x);
void   *asm_memcpy(void *dest, const void *src, size_t n);
int     asm_setjmp(asm_jmp_buf env);
void    asm_longjmp(asm_jmp_buf env, int val);


/*
int64_t std_add(int64_t a, int64_t b);
int     std_popcnt(uint64_t x);
void   *std_memcpy(void *dest, const void *src, size_t n);
*/