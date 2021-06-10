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

void set_print(set *s, size_t index, size_t cs, size_t b, FILE *output_file, const ram_image *ri) {

    int line_length = 19 + pow(2, b + 1);

    if (s->next == NULL) {
        fprintf(output_file, "|");
        for (int i = 0; i < line_length / 2 - 5; i++) fprintf(output_file, " ");
        fprintf(output_file, "EMPTY SET");
        for (int i = 0; i < line_length / 2 - 5; i++) fprintf(output_file, " ");
        fprintf(output_file, "|\n");
        return;
    } 

    node *current = s->next;
    while (current != NULL) {
        u32 address = ((current->tag << cs) + index) << b;
        size_t B = pow(2, b);

        fprintf(output_file, "| 0x%08X | 0x", address);
        for (size_t i = 0; i < B; i++) {
            fprintf(output_file, "%02X", ri->data[address + i]);
        }
        fprintf(output_file, " |\n");
        current = current->next;
    }

    return;
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