#include "../include/libs.h"
#include "../include/set.h"
#include "../include/ram.h"
#include "../include/cache.h"
#include "../include/pargser.h"

#define BUFFER_SIZE 100

static const char *const usage = "Usage: ./your_simulator -L1s <L1s> -L1E <L1E> -L1b <L1b> -L2s <L2s> -L2E <L2E> -L2b <L2b> -t <tracefile>";

// Cache Sizes
static int L1s, L1E, L1b, L2s, L2E, L2b;
static const char *trace_name = NULL;
static const char *ram_name = NULL;

void take_arguments(int argc, const char *argv[]);
void run_tests();

int main(int argc, char *argv[]) {

    pargser(argc, argv, 
        "-L1s%d-L1E%d-L1b%d-L2s%d-L2E%d-L2b%d-t%*-r%*",
        &L1s, &L1E, &L1b, &L2s, &L2E, &L2b, &trace_name, &ram_name);

    run_tests();

    return 0;
}

void run_tests() {

    FILE *trace = fopen(trace_name, "r");

    if (trace == NULL) {
        fprintf(stderr, "File does not exist!\n");
        exit(1);
    }

    ram_image ri = ram_create(ram_name);

    cache L1I = cache_create(L1s, L1E, L1b);
    cache L1D = cache_create(L1s, L1E, L1b);
    cache L2 = cache_create(L2s, L2E, L2b);

    char *buffer = malloc(BUFFER_SIZE);
    while (fgets(buffer, BUFFER_SIZE, trace) != NULL) {
        char op = buffer[0];

        char *temp = NULL;
        u32 address = strtoul(buffer + 2, &temp, 16);
        size_t size = strtoul(temp + 2, &temp, 10);
        u64 d = 0;
        if (op == 'M' || op == 'S') d = strtoull(temp + 2, NULL, 16);

        u8 *data = NULL;
        if (op == 'M' || op == 'S') {
            data = malloc(size);

            for (size_t i = 0; i < size; i++) {
                data[i] = d % 256;
                d /= 256;
            }
        }
        
        if (op == 'L') {
            u8 result = cache_find(&L1D, address, 1);
            if (result != 0x4) cache_find(&L2, address, 1);
        } else if (op == 'I') {
            u8 result = cache_find(&L1I, address, 1);
            if (result != 0x4) cache_find(&L2, address, 1);
        } else if (op == 'S') {
            cache_find(&L1D, address, 0);
            cache_find(&L2, address, 0);
            ram_write_data(&ri, address, data, size);
        } else {
            u8 result = cache_find(&L1D, address, 1);
            if (result != 0x4) cache_find(&L2, address, 1);
            cache_find(&L1D, address, 0);
            cache_find(&L2, address, 0);
            ram_write_data(&ri, address, data, size);
        }
        if (data != NULL) free(data);
    }

    free(buffer);
    fclose(trace);

    cache_print("L1I", &L1I, &ri);
    cache_print("L1D", &L1D, &ri);
    cache_print("L2", &L2, &ri);

    printf("L1I-hits:%10zu     L1I-misses:%10zu     L1I-evictions:%10zu\n", L1I.hit, L1I.miss, L1I.eviction);
    printf("L1D-hits:%10zu     L1D-misses:%10zu     L1D-evictions:%10zu\n", L1D.hit, L1D.miss, L1D.eviction);
    printf("L2-hits:%11zu     L2-misses:%11zu     L2-evictions:%11zu\n", L2.hit, L2.miss, L2.eviction);

    ram_update(&ri);
    ram_free(&ri);
    cache_free(&L1I);
    cache_free(&L1D);
    cache_free(&L2);

    return;
}