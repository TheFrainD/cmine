#include "chunk.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <log.h>

#include "utils/noise.h"

void chunk_create(chunk *self, vec3s position) {
    self->position = position;
    self->blocks = (block *)malloc(sizeof(block) * CHUNK_VOLUME);
    if (!self->blocks) {
        log_error("Could not create chunk!");
        return;
    }
}

void chunk_destroy(chunk *self) {
    free(self->blocks);
}