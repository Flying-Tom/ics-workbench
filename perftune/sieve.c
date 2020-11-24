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
    int *p = primes, cnt = 0;
    /*
    for (int i = 0; i <= n; i++)
        is_prime[i] = true;*/
    is_prime[1] = false;
    for (int i = 2; i <= n; i++)
    {
        if (is_prime[i])
        {
            *p++ = i;
            cnt++;
        }
        for (int j = 1; j <= cnt && i * *(p - cnt - 1 + j) <= n; j++)
        {
            is_prime[i * *(p - cnt - 1 + j)] = false;
            if (i % *(p - cnt - 1 + j) == 0)
                break;
        }
    }
    /*
    int *p = primes;
    for (int i = 2; i <= n; i++)
        if (is_prime[i])
        {
            *p++ = i;
        }
    *p = 0;*/
    return primes;
}
