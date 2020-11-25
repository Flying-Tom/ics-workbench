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
    int *p = primes, temp;
    is_prime[2] = true;
    for (int i = 4; i <= n; i += 2)
    {
        is_prime[i] = false;
        is_prime[i - 1] = true;
    }

    *p++ = 2;
    for (int i = 3; i <= n; i += 2)
    {
        if (is_prime[i])
            *p++ = i;
        for (int j = 2; i * primes[j-1] <= n; j++)
        {
            is_prime[i * prime[j-1]] = false;
            if (!(i % prime[j-1]))
                break;
        }
    }
    return primes;
}
