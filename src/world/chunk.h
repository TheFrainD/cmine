#ifndef CMINE_CHUNK_H_
#define CMINE_CHUNK_H_

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "config.h"
#include "block.h"

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32
#define CHUNK_VOLUME (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

typedef struct {
    block blocks[CHUNK_VOLUME];
    vec3s position;
} chunk;

void chunk_create(chunk *self, vec3s position);
void chunk_destroy(chunk *self);

#endif // CMINE_CHUNK_H_