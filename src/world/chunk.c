#include "chunk.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <log.h>

void chunk_create(chunk *self, vec3s position) {
    self->position = position;
    if (!self->blocks) {
        log_error("Could not create chunk!");
        return;
    }
}

void chunk_destroy(chunk *self) {
    free(self->blocks);
}