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
    int *p = primes, *prime = primes - 1, temp;
    for (int i = 0; i <= n; i++)
        is_prime[i] = true;
    memset(is_prime,0,sizeof(is_prime));
    is_prime[1] = false;
    for (int i = 2; i <= n; i++)
    {
        if (is_prime[i])
            *p++ = i;

        for (int j = 1; j <= p - prime && i * *(prime + j) <= n; j++)
        {
            is_prime[i * *(prime + j)] = false;
            if (i % *(prime + j) == 0)
                break;
        }
    }
    return primes;
}
