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
    memset(is_prime, 1, sizeof(is_prime));
    is_prime[1] = false;
    for (int i = 2; i <= n; i++)
    {
        if (is_prime[i])
            *p++ = i;
        temp = i * prime[1];
        for (int j = 1; temp <= n; j++)
        {
            is_prime[temp] = false;
            if ( !(i % prime[j] ))
                break;
            temp = i * prime[j+1];
        }
    }
    return primes;
}
