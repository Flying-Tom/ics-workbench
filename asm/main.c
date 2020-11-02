#include "asm.h"
#include <assert.h>
#include <stdio.h>

int a[8]={5,3,2,1,4};
int b[8]={};
int main()
{
    asm_jmp_buf buf;
    int r = asm_setjmp(buf);
    if (r == 0)
    {
        assert(asm_add(1234, 5678) == 6912);
        assert(asm_popcnt(0x0123456789abcdefULL) == 32);
        assert(asm_add(0x100000000, 0x100000000) == std_add(0x100000000, 0x10000000));
        asm_memcpy(b,a,sizeof(int) * 4);
        // TODO: add more tests here.
        asm_longjmp(buf, 123);
    }
    else
    {
        assert(r == 123);
        printf("PASSED.\n");
    }
}
