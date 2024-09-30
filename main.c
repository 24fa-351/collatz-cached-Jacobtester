//main.c
//Jacob Gray
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "collatz.h"
#include "cache.h"
// make
// gcc -o main main.c
// gcc -o main main.c collatz.c cache.c
// (n is number of values to test)
// (min is minimum value to test)
// (max is maximum value to test)
// time ./main n min max method chache_size
// implement LRU and LFU cache

//Random Num
unsigned long long random_number(unsigned long long min, unsigned long long max)
{
    return min + (rand() % (max - min + 1));
}


int main(int argc, char *argv[])
{
    unsigned long long n = atoll(argv[1]);
    unsigned long long min = atoll(argv[2]);
    unsigned long long max = atoll(argv[3]);
    unsigned long long size = 0;
    unsigned long long method = 0;

    if(argc == 6)
    {
        if(strcmp(argv[4], "LRU") == 0)
        {
            method = 1;
            size = atoll(argv[5]);
        }
        else if(strcmp(argv[4], "LFU") == 0)
        {
            method = 2;
            size = atoll(argv[5]);
        }
        else
        {
            method = 0;
        }
    }

    srand(time(NULL));
    cache_init(method, size);

    for(int ix = 0;ix < n; ix++)
    {
        unsigned long long num_random = random_number(min, max);
        unsigned long long current_count = collatz_r(num_random);
        printf("Collatz of %llu is %llu\n", num_random, current_count);
    }

    unsigned long long access_amount = cache_hits_get() + cache_misses_get();

    if(access_amount > 0)
    {
        double hit_rate = ((double)cache_hits_get() * 100.0) / (double)access_amount;
        printf("Hit rate: %.2f%%\n", hit_rate);
    }
    else printf("Hit rate: 0%%");

    cache_free();
    //end
    return 0;
}
