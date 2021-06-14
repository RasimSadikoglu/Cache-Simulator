#include "set.h"

struct node {
    u32 tag;
    node *next;
};

set set_create(size_t E) {
    set s;
    s.E = E;
    s.valid_blocks = 0;
    s.next = NULL;
    return s;
}

void set_pop(set *s) {

    node *next = s->next->next;

    free(s->next);

    s->next = next;

    s->valid_blocks--;

    return;
}

u8 set_push(set *s, u32 tag) {
    u8 evict = 0;
    if (s->valid_blocks == s->E) {
        evict = 1;
        set_pop(s);
    }

    s->valid_blocks++;

    node *block = malloc(sizeof(node));
    block->tag = tag;
    block->next = NULL;

    if (s->next == NULL) {
        s->next = block;
        return evict;
    }

    node *current = s->next;
    while (current->next != NULL) current = current->next;

    current->next = block;

    return evict;
}

u8 set_find(set *s, u32 tag, u8 push) {

    node *current = s->next;
    while (current != NULL) {
        if (current->tag == tag) {
            return 0x4;
        }
        current = current->next;
    }

    if (push) return set_push(s, tag) | 0x2;

    return 0x2;
}

int set_print(set *s, size_t index, size_t ss, size_t b, FILE *output_file, const ram_image *ri) {

    if (s->next == NULL) return 1;

    size_t S = pow(2, ss), B = pow(2, b);
    int index_length = (ss + 3) / 4;

    node *current = s->next;
    while (current != NULL) {
        u32 address = ((current->tag << ss) + index) << b;

        fprintf(output_file, "| 0x%0*lX | 0x%08X | 0x", index_length, index, current->tag);
        u8 *data = ram_read_data(ri, address);
        for (size_t i = 0; i < B; i++) fprintf(output_file, "%02X", data[i]);
        fprintf(output_file, " |\n");
        current = current->next;
    }

    return 0;
}

void set_free(set *s) {
    
    node *current = s->next;
    while (current != NULL) {
        node *temp = current->next;

        free(current);

        current = temp;
    }

    return;
}