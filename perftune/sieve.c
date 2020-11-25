#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define N 10000000

static bool is_prime[N];
static int primes[N];

int *sieve(int n)
{
    assert(n + 1 < N);
    memset(is_prime,true,sizeof(is_prime));
    int itemp = 5, jtemp = 0, cnt = 1;
    primes[0] = 2;
    primes[1] = 3;
    for (int i = 0; itemp <= n; i++)
    {
        if (is_prime[i])
            primes[++cnt ] = itemp;
        for (int j = 2; j <= cnt && itemp * primes[j] <= n; j++)
        {
            is_prime[i * primes[j]] = false;
            if (!(itemp % primes[j]))
                break;
        }
        itemp += 2 + ((i & 1) << 1);
    }
    return primes;
}
