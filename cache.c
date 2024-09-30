//cache.c
#include "cache.h"
#include "collatz.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct cache_node
{
    unsigned long long key;
    unsigned long long value;
    struct cache_node *next;
    struct cache_node *prev;
    unsigned long long frequency;
} cache_node;

//methods 0 = no cache, 1 = LRU, 2 = LFU
static cache_node *cache_head = NULL;
static cache_node *cache_tail = NULL;
static unsigned long long cache_method;
static unsigned long long cache_size_max;
static unsigned long long cache_size_current = 0;
static unsigned long long cache_hits = 0;
static unsigned long long cache_misses = 0;

void cache_init(unsigned long long method, unsigned long long size)
{
    cache_method = method;
    cache_size_max = size;
    cache_size_current = 0;
    cache_hits = 0;
    cache_misses = 0;
    cache_head = NULL;
    cache_tail = NULL;
}

bool cache_has(unsigned long long key_in)
{
    if(cache_method == 0)
    {
        return false;
    }
    cache_node *current = cache_head;

    while(current != NULL)
    {
        if(current->key == key_in)
        {
            cache_hits++;
            //LRU
            if(cache_method == 1)
            {
                //moving to front
                if(current != cache_head)
                {
                    //remove from current position in the list
                    if(current->prev != NULL) current->prev->next = current->next;
                    if(current->next != NULL) current->next->prev = current->prev;
                    else cache_tail = current->prev;

                    //add to front
                    current->prev = NULL;
                    current->next = cache_head;
                    cache_head->prev = current;
                    cache_head = current;
                }
            }
            //LFU
            else if(cache_method == 2)
            {
                current->frequency++;
            }
            return true;
        }
        current = current->next;
    }
    cache_misses++;
    return false;
}

unsigned long long cache_value_for(unsigned long long key_in)
{
    cache_node *current = cache_head;
    while(current != NULL)
    {
        if(current->key == key_in)
        {
            return current->value;
        }
        current = current->next;
    }

    return 0;
}

void cache_insert(unsigned long long num, unsigned long long count)
{
    if(cache_method == 0)
    {
        return;
    }

    cache_node *current = cache_head;

    //If alread in cache, return
    while(current != NULL)
    {
        if(current->key == num)
        {
            current->value = count;
            return;
        }
        current = current->next;
    }

    //CACHE FULL REMOVAL PROCESS
    if(cache_size_current == cache_size_max)
    {
        //LRU
        if(cache_method == 1)
        {
            cache_node *node_removed = cache_tail;

            if(node_removed != NULL)
            {
                if(node_removed->prev != NULL)
                {
                    node_removed->prev->next = NULL;
                    cache_tail = node_removed->prev;
                }
                //If it was the only node
                else
                {
                    cache_head = NULL;
                    cache_tail = NULL;
                }

                free(node_removed);
                cache_size_current--;
            }
        }
        //LFU
        else if(cache_method == 2)
        {
            cache_node *node_removed = cache_head;
            unsigned long long min_frequency = node_removed->frequency;
            cache_node *current = cache_head;

            //Find node with lowest frequency
            while(current != NULL)
            {
                if(current->frequency < min_frequency)
                {
                    min_frequency = current->frequency;
                    node_removed = current;
                }
                current = current->next;
            }

            if(node_removed->prev != NULL) node_removed->prev->next = node_removed->next;
            else cache_head = node_removed->next;
            if(node_removed->next != NULL) node_removed->next->prev = node_removed->prev;
            else cache_tail = node_removed->prev;
            free(node_removed);
            cache_size_current--;
        }
    }

    //INSERT NEW NODE PROCESS
    cache_node *new_node = malloc(sizeof(cache_node));
    
    if(new_node == NULL)
    {
        printf("Error allocating memory for cache node\n");
        return;
    }

    new_node->key = num;
    new_node->value = count;
    new_node->frequency = 1;
    new_node->next = cache_head;
    new_node->prev = NULL;

    if(cache_head != NULL) cache_head->prev = new_node;
    else cache_tail = new_node;

    cache_head = new_node;
    cache_size_current++;
}

void cache_free()
{
    cache_node *current = cache_head;

    while(current != NULL)
    {
        cache_node *next = current->next;
        free(current);
        current = next;
    }

    cache_head = NULL;
    cache_tail = NULL;
    cache_size_current = 0;
}

unsigned long long cache_hits_get()
{
    return cache_hits;
}

unsigned long long cache_misses_get()
{
    return cache_misses;
}