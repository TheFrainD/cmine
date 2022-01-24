#ifndef CMINE_CHUNK_H_
#define CMINE_CHUNK_H_

#include "config.h"
#include "block.h"

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32
#define CHUNK_VOLUME (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

typedef struct {
    block* blocks;
} chunk;

void chunk_create(chunk *self);
void chunk_destroy(chunk *self);

#endif // CMINE_CHUNK_H_