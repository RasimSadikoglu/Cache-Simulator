#ifndef LIBS_H
#define LIBS_H

#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define printchar(OUT_FILE, CHAR, COUNT) for (int i = 0; i < COUNT; i++) fprintf(OUT_FILE, CHAR); fprintf(OUT_FILE, "\n")

#ifndef _U64
#define _U64
#if (ULONG_MAX == 0xFFFFFFFFFFFFFFFF)
typedef unsigned long u64;
#else
typedef unsigned long long u64;
#endif
#endif

#ifndef _U32
#define _U32
#if (UINT_MAX == 0xFFFFFFFF)
typedef unsigned int u32;
#else
typedef unsigned long u32;
#endif
#endif

#ifndef _U8
#define _U8
typedef unsigned char u8;
#endif

typedef struct node node;

#endif // LIBS_H