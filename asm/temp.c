#include <stdint.h>
#include <stdio.h>
#include <assert.h>

int64_t asm_add(int64_t a, int64_t b)
{
    int64_t temp ;
    asm(
        "addq %1, %2;"
        "movq %2, %0;"
        : "=r"(temp)
        : "r"(a),"r"(b)
        );
    return temp;
}
int inc(int x)
{
    asm("incl %[t];"
        : [ t ] "+r"(x));
    return x;
}
int64_t std_add(int64_t a,int64_t b)
{
    return a+b;
}

int main()
{
    assert(inc(1) == 2);
    //printf("%d\n", asm_add(0x10002, 0x1000000));
    assert(asm_add(114, 115) == 229);
    int64_t fff = std_add(0x100000000, 0x100000000);
    fff++;
}