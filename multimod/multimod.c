#include <stdint.h>

uint64_t mod(uint64_t x, uint64_t y)
{
    while (x>=y)
        x-=y;
    return x;
}

uint64_t addmod(uint64_t a, uint64_t b, uint64_t m)
{
    uint64_t sum = 0;
    while (b)
    {
        sum = a ^ b;
        b = (a & b) << 1;
        a = mod(sum,m);
    }
    return a;
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m)
{
    uint64_t ans = 0;
    while (a)
    {
        if (a & 1)
            ans = addmod(ans, b, m);
        b <<= 1;
        a >>= 1;
    }
    return ans;
}