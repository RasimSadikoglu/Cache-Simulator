#ifndef RAM_H
#define RAM_H

#include "libs.h"

typedef struct {
    size_t size;
    u8 *data;
} ram_image;

ram_image ram_create();
u8* ram_read_data(const ram_image *ri, size_t address);
void ram_write_data(ram_image *ri, size_t address, u8 *data, size_t size_of_data);
void ram_update(const ram_image *ri);
void ram_free(ram_image *ri);

#endif // RAM_H