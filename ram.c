#include "ram.h"

#define BUFFER_SIZE 0x100

ram_image ram_create(const char *ram_name) {

    FILE *ram_file = fopen(ram_name, "rb");

    if (ram_file == NULL) {
        fprintf(stderr, "File not found!\n");
        exit(EXIT_FAILURE);
    }

    fseek(ram_file, 0, SEEK_END);
    size_t size = ftell(ram_file);
    rewind(ram_file);

    ram_image ri;
    ri.size = size;
    ri.data = malloc(size);

    fread(ri.data, 1, size, ram_file);

    fclose(ram_file);

    return ri;
}

u8* ram_read_data(const ram_image *ri, size_t address) {
    if (address > ri->size) {
        fprintf(stderr, "Invalid memory access!\nAddress:  0x%08lX\nRam Size: 0x%08lX\n", address, ri->size);
        exit(EXIT_FAILURE);
    }

    return ri->data + address;
}

void ram_write_data(ram_image *ri, size_t address, u8 *data, size_t size_of_data) {
    if (address > ri->size) {
        fprintf(stderr, "Invalid memory access!\nAddress:  0x%08lX\nRam Size: 0x%08lX\n", address, ri->size);
        exit(EXIT_FAILURE);
    }

    memcpy(ri->data + address, data, size_of_data);

    return;
}

void ram_update(const ram_image *ri) {

    FILE *ram_file = fopen("updated_ram.dat", "wb");

    fwrite(ri->data, 1, ri->size, ram_file);

    fclose(ram_file);

    return;
}

void ram_free(ram_image *ri) {
    free(ri->data);
    return;
}