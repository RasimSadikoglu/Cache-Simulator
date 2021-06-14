#include "cache.h"

cache cache_create(size_t s, size_t E, size_t b) {

    if (E == 0) {
        fprintf(stderr, "Cache with size 0, is kinda useless! (E cannot be zero!)\n");
        exit(EXIT_FAILURE);
    }

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
    else set_index = (address << (32 - c->b - c->s)) >> (32 - c->s);

    u8 result = set_find(c->sets + set_index, tag, push);

    if (result & 0x4) c->hit++;
    if (result & 0x2) c->miss++;
    if (result & 0x1) c->eviction++;

    return result;
}

void cache_print(const char *name, const cache *c, FILE *output_file, const ram_image *ri) {

    if (output_file == NULL) output_file = stdout;

    size_t S = pow(2, c->s), B = pow(2, c->b);

    int name_length = strlen(name);

    int set_index_length = (c->s + 3) / 4 + 4 + (c->s == 0);
    int tag_length = 12;
    int data_length = B * 2 + 4;
    int line_length = set_index_length + tag_length + data_length + 4;

    int a1 = (line_length - 2 - name_length);

    fprintf(output_file, "<%*s%s%*s>\n", a1 / 2, "", name, (a1 + 1) / 2, "");
    fprintf(output_file, "| %-*s| %-*s| %-*s|\n", set_index_length - 1, "Set", tag_length - 1, "Tag", data_length - 1, "Data");
    printchar(output_file, "=", line_length);

    for (size_t i = 0; i < S; i++) {
        if(set_print(c->sets + i, i, c->s, c->b, output_file, ri)) {
            a1 = line_length - 11;
            fprintf(output_file, "|%*s%s%*s|\n", a1 / 2, "", "EMPTY SET", (a1 + 1) / 2, "");
        }
        printchar(output_file, "=", line_length);
    }

    fprintf(output_file, "\n");
    return;
}

void cache_free(cache *c) {

    size_t S = pow(2, c->s);
    for (size_t i = 0; i < S; i++) set_free(c->sets + i);

    free(c->sets);

    return;
}