#include <stdint.h>

uint64_t addmod(uint64_t a, uint64_t b, uint64_t m)
{
    uint64_t sum = 0;
    while (b)
    {
        sum = a ^ b;
        b = (a & b) << 1;
        a = sum & m;
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
}