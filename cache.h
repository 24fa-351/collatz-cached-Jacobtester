// cache.h
#ifndef CACHE_H
#define CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void cache_init(unsigned long long method, unsigned long long size);

bool cache_has(unsigned long long key_in);

unsigned long long cache_value_for(unsigned long long key_in);

void cache_insert(unsigned long long num, unsigned long long count);

void cache_free();

unsigned long long cache_hits_get();

unsigned long long cache_misses_get();
#endif