#include "chunk.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <log.h>

#include "utils/noise.h"

void chunk_create(chunk *self) {
    noise_seed(time(NULL));

    self->blocks = (block *)malloc(sizeof(block) * CHUNK_VOLUME);
    if (!self->blocks) {
        log_error("Could not create chunk!");
        return;
    }

    for (i32 y = 0; y < CHUNK_SIZE_Y; y++) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; z++) {
            for (i32 x = 0; x < CHUNK_SIZE_X; x++) {
                if (y <= perlin2d(x, z, 0.06f, 3) * 20.0f) {
                    self->blocks[(y * CHUNK_SIZE_Z + z) * CHUNK_SIZE_X + x] = y <= 5 ? 2 : 1;
                } else {
                    self->blocks[(y * CHUNK_SIZE_Z + z) * CHUNK_SIZE_X + x] = 0;
                }
            }
        }
    }
}

void chunk_destroy(chunk *self) {
    free(self->blocks);
}