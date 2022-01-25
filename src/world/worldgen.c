#include "worldgen.h"

#include <stdlib.h>
#include <time.h>

#include <log.h>

#include "utils/noise.h"

#define _3D_TO_1D(x, y, z, depth, width) (((y) * (depth) + (z)) * (width) + (x))
#define approx(x, ampl) ((rand() % ampl) - ampl / 2.0f)

#define WORLD_VOL (state.size_x * state.size_y * state.size_z)

typedef struct {
    chunk *chunks;
    mesh *meshes;
    i32 size_x;
    i32 size_y;
    i32 size_z;
} world_state;

static b8 initalized = FALSE;
static world_state state;

static f32 get_height(f32 x, f32 y, f32 scale, i32 octaves, f32 persistance, f32 lacunarity) {
    if (scale <= 0.0f) {
        scale = 0.0001f;
    }

    f32 amplitude = 1.0f;
    f32 frequency = 1.0f;
    f32 height = 0.0f;

    for (int i = 0; i < octaves; i++) {
        // height += noise2d(x / scale * frequency, y / scale * frequency) * amplitude;
        height += perlin(x / scale * frequency, y / scale * frequency) * amplitude;
        frequency *= lacunarity;
        amplitude *= persistance;
    }

    return ((height + 1.0f) * (f32)CHUNK_SIZE_Y / 2.0f) + 40.0f;
}

b8 world_init(i32 size_x, i32 size_y, i32 size_z) {
    if (initalized) {
        return FALSE;
    }

    srand(time(NULL));
    noise_seed(time(NULL));

    state.size_x = size_x;
    state.size_y = size_y;
    state.size_z = size_z;
    state.chunks = (chunk *)malloc(sizeof(chunk) * WORLD_VOL);
    state.meshes = (mesh *)malloc(sizeof(mesh) * WORLD_VOL);
    if (!state.chunks || !state.meshes) {
        return FALSE;
    }

    initalized = TRUE;

    log_trace("World initialized");

    return TRUE;
}

void world_gen_chunks() {
    if (!initalized) {
        return FALSE;
    }
    
    log_info("Worldgen init...");

    i32 count = 0;
    f32 xoffset = rand();
    f32 zoffset = rand();
    for (i32 yy = 0; yy < state.size_y; yy++) {
        for (i32 zz = 0; zz < state.size_z; zz++) {
            for (i32 xx = 0; xx < state.size_x; xx++) {
                chunk *c = &state.chunks[_3D_TO_1D(xx, yy, zz, state.size_z, state.size_x)];
                chunk_create(c, (vec3s){xx * CHUNK_SIZE_X,
                                        yy * CHUNK_SIZE_Y,
                                        zz * CHUNK_SIZE_Z});
                
                log_info("Generating chunk #%i", count);
                count ++;
                
                for (i32 y = 0; y < CHUNK_SIZE_Y; y++) {
                    for (i32 z = 0; z < CHUNK_SIZE_Z; z++) {
                        for (i32 x = 0; x < CHUNK_SIZE_X; x++) {
                            i32 samplex = x + xx * CHUNK_SIZE_X + xoffset;
                            i32 samplez = z + zz * CHUNK_SIZE_Z + zoffset;
                            i32 sampley = y + yy * CHUNK_SIZE_Y;

                            if (sampley <= get_height(samplex, samplez, 50, 4, 0.8f, 1.4f)) {
                                c->blocks[_3D_TO_1D(x, y, z, CHUNK_SIZE_Z, CHUNK_SIZE_X)] = 1;

                                if (sampley <= 38 || sampley >= 70) {
                                    c->blocks[_3D_TO_1D(x, y, z, CHUNK_SIZE_Z, CHUNK_SIZE_X)] = 2;
                                } else if (sampley >= 48 && sampley <= 53) {
                                    c->blocks[_3D_TO_1D(x, y, z, CHUNK_SIZE_Z, CHUNK_SIZE_X)] = 4;
                                }
                            } else if (sampley == 50) {
                                c->blocks[_3D_TO_1D(x, y, z, CHUNK_SIZE_Z, CHUNK_SIZE_X)] = 15;
                            } else {
                                c->blocks[_3D_TO_1D(x, y, z, CHUNK_SIZE_Z, CHUNK_SIZE_X)] = 0;
                            }

                        }
                    }
                }

                
            }
        }
    }

    count = 0;
    for (i32 yy = 0; yy < state.size_y; yy++) {
        for (i32 zz = 0; zz < state.size_z; zz++) {
            for (i32 xx = 0; xx < state.size_x; xx++) {
                log_info("Generating chunk mesh #%i", count);
                count ++;
                chunk *c = &state.chunks[_3D_TO_1D(xx, yy, zz, state.size_z, state.size_x)];
                chunk *neighbouring[6];
                neighbouring[UP] = yy < (CHUNK_SIZE_Y - 1) ? &state.chunks[_3D_TO_1D(xx, yy+1, zz, state.size_z, state.size_x)] : NULL;
                neighbouring[DOWN] = yy > 0 ? &state.chunks[_3D_TO_1D(xx, yy-1, zz, state.size_z, state.size_x)] : NULL;
                neighbouring[RIGHT] = xx < (CHUNK_SIZE_X - 1) ? &state.chunks[_3D_TO_1D(xx+1, yy, zz, state.size_z, state.size_x)] : NULL;
                neighbouring[LEFT] = xx > 0 ? &state.chunks[_3D_TO_1D(xx-1, yy, zz, state.size_z, state.size_x)] : NULL;
                neighbouring[FRONT] = zz < (CHUNK_SIZE_Z - 1) ? &state.chunks[_3D_TO_1D(xx, yy, zz+1, state.size_z, state.size_x)] : NULL;
                neighbouring[BACK] = zz > 0 ? &state.chunks[_3D_TO_1D(xx, yy, zz-1, state.size_z, state.size_x)] : NULL;

                chunk_gen_mesh(c, &state.meshes[_3D_TO_1D(xx, yy, zz, state.size_z, state.size_x)], neighbouring);
            }
        }
    }
}

void world_draw(shader *shader) {
    if (!initalized) {
        return;
    }

    mat4s model;
    for (int i = 0; i < WORLD_VOL; i++) {
        if (state.meshes[i].used) {
            model = glms_translate_make(state.chunks[i].position);
            shader_mat4(shader, "model", model);
            mesh_draw(&state.meshes[i]);
        }
    }
}
