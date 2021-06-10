#ifndef SET_H
#define SET_H

#include "libs.h"
#include "ram.h"

typedef struct {
    size_t E;
    size_t valid_blocks;
    node *next;
} set;

set set_create(size_t E);
u8 set_find(set *s, u32 tag, u8 push);
void set_print(set *s, size_t index, size_t cs, size_t b, FILE *output_file, const ram_image *ri);
void set_free(set *pq);

#endif // SET_H