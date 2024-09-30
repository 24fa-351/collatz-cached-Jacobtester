//collatz.c
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "cache.h"
#include "collatz.h"

//collatz
unsigned long long collatz_r(unsigned long long num)
{
    if(num == 1)
    {
        return 0;
    }
    //
    if(cache_has(num))
    {
        return cache_value_for(num);
    }
    //
    unsigned long long new_num;

    if(num % 2 == 0)
    {
        new_num = num / 2;
    }
    else
    {
        new_num = 3 * num + 1;
    }

    if(new_num == 1) return 1;

    unsigned long long count = 1 + collatz_r(new_num);
    //
    cache_insert(num, count);
    //
    return count;
}
