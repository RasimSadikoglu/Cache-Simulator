#include "cache.h"

cache cache_create(size_t s, size_t E, size_t b) {
    cache c;
    c.miss = 0; c.hit = 0; c.eviction = 0;
    c.s = s; c.E = E; c.b= b;

    size_t S = pow(2, s);

    c.sets = malloc(sizeof(set) * S);
    for (size_t i = 0; i < S; i++) c.sets[i] = set_create(E);

    return c;
}

u8 cache_find(cache *c, u32 address, u8 push) {

    u32 tag = address >> (c->s + c->b);
    u32 set_index;
    if (c->s == 0) set_index = 0;
    else set_index = (address << (32 - c->b + c->s)) >> (32 - c->s);

    u8 result = set_find(c->sets + set_index, tag, push);

    if (result & 0x4) c->hit++;
    if (result & 0x2) c->miss++;
    if (result & 0x1) c->eviction++;

    return result;
}

void cache_print(const char *name, const cache *c, FILE *output_file, const ram_image *ri) {
    if (output_file == NULL) output_file = stdout;

    int line_length = 19 + pow(2, c->b + 1);

    fprintf(output_file, "<");
    for (int i = 0; i < line_length / 2 - strlen(name) / 2 - 1; i++) fprintf(output_file, " ");
    fprintf(output_file, "%s", name);
    for (int i = 0; i < line_length / 2 - strlen(name) / 2 - 1 + (strlen(name) % 2 == 0); i++) fprintf(output_file, " ");
    fprintf(output_file, ">\n");

    size_t S = pow(2, c->s);
    for (size_t i = 0; i < S; i++) {
        for (int i = 0; i < line_length; i++) fprintf(output_file, "=");
        fprintf(output_file, "\n");
        set_print(c->sets + i, i, c->s, c->b, output_file, ri);
    }
    for (int i = 0; i < 19 + pow(2, c->b + 1); i++) fprintf(output_file, "=");
    fprintf(output_file, "\n\n\n");

    return;
}

void cache_free(cache *c) {

    size_t S = pow(2, c->s);
    for (size_t i = 0; i < S; i++) set_free(c->sets + i);

    free(c->sets);

    return;
}