#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static int flags[N / 96 + 1];
static int primes[N];

int *sieve(int n)
{
    assert(n + 1 < N);
    memset(is_prime, 1, sizeof(is_prime));
    int itemp = 5, jtemp = 0, cnt = 1;
    primes[0] = 2;
    primes[1] = 3;
    for (int i = 0; itemp <= n; i++)
    {
        if (!(flags[i >> 5] & (1 << (i & 31))))
            primes[++cnt - 1] = itemp;
        for (int j = 2; j <= cnt && itemp * prime[j] <= n; j++)
        {
            jtemp = (itemp * primes[j] - 5) >> 1;
            jtemp -= jtemp / 3;
            flags[jtemp >> 5] |= 1 << (jtemp & 31);
            if (!(itemp % primes[j]))
                break;
        }
        itemp += 2 + ((i & 1) << 1);
    }
    return primes;
}
