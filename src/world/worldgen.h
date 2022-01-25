#ifndef CMINE_WORLDGEN_H_
#define CMINE_WORLDGEN_H_

#include "config.h"
#include "world/chunk.h"
#include "world/chunkmesh.h"
#include "gfx/shader.h"

b8 world_init();
void world_gen_chunks();
void world_draw(shader *shader);

#endif // CMINE_WORLDGEN_H_