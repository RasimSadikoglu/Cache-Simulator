#ifndef CACHE_H
#define CACHE_H

#include "set.h"
#include "ram.h"

typedef struct {
    size_t miss, hit, eviction;
    size_t s, E, b;
    set *sets;
} cache;

cache cache_create(size_t s, size_t E, size_t b);
u8 cache_find(cache *c, u32 address, u8 push);
void cache_print(const char *name, const cache *c, FILE *output_file, const ram_image *ri);
void cache_free(cache *c);

#endif // CACHE_H