#include <stdint.h>
#define MAXN 0xffffffffffffffff

uint64_t mod(uint64_t x, uint64_t y)
{
    uint64_t xtemp = x >> 1,ytemp = y;
    while (xtemp >= ytemp)
        ytemp <<= 1;
    while (ytemp >= y)
    {
        if(x >= ytemp)
            x -= ytemp;
        ytemp >>= 1;
    }
    return x;
}

uint64_t addmod(uint64_t a, uint64_t b, uint64_t m)
{
    uint64_t t=mod(a+b,m)+1;
    if(a+b<a || a+b<b)
        return mod(mod(t, m) + mod(MAXN, m), m);
    else
        return mod(mod(a, m) + mod(b, m), m); 
}

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m)
{
    uint64_t ans = 0,cnt=0;
    while (a)
    {
        if (a & 1)
        {    
            uint64_t btemp=b,cnttemp=cnt;
            while(cnttemp--)
                btemp = addmod(btemp, btemp, m);
            ans = addmod(ans, btemp, m);
        }
        a >>= 1;
        cnt++;
    }
    return ans;
}