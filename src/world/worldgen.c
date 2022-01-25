#include "worldgen.h"

#include <stdlib.h>
#include <time.h>

#include <log.h>
#include <noise1234.h>

#define _3D_TO_1D(x, y, z, depth, width) (((y) * (depth) + (z)) * (width) + (x))

#define WORLD_SIZE_X 8
#define WORLD_SIZE_Y 4
#define WORLD_SIZE_Z 8
#define WORLD_VOL (WORLD_SIZE_X * WORLD_SIZE_Y * WORLD_SIZE_Z)

typedef struct {
    chunk chunks[WORLD_VOL];
    mesh meshes[WORLD_VOL];
} world_state;

static b8 initalized = FALSE;
static world_state state;

static inline f32 noise_norm(float x, float y) {
    return (noise2(x, y) + 1.0f) / 2.0f;
}

static f32 get_height(f32 x, f32 y, f32 scale, i32 octaves, f32 persistance, f32 lacunarity) {
    f32 amplitude = 1.0f;
    f32 amplitudes = 0.0f;
    f32 frequency = 1.0f;
    f32 height = 0.0f;
    
    for (i32 i = 0; i < octaves; i++) {
        f32 sample_x = x / scale * frequency;
        f32 sample_y = y / scale * frequency;

        height += noise_norm(sample_x, sample_y) * amplitude;

        amplitudes += amplitude;
        amplitude *= persistance;
        frequency *= lacunarity;
    }

    height = height / amplitudes;
    height = powf(height * 1.2f, 1.5f) / 1.2f;

    return height * CHUNK_SIZE_Y * WORLD_SIZE_Y;
}

b8 world_init() {
    if (initalized) {
        return FALSE;
    }

    srand(time(NULL));;
    // state.chunks = (chunk *)malloc(sizeof(chunk) * WORLD_VOL);
    // state.meshes = (mesh *)malloc(sizeof(mesh) * WORLD_VOL);
    if (!state.chunks || !state.meshes) {
        return FALSE;
    }

    initalized = TRUE;

    log_trace("World initialized");

    return TRUE;
}

void world_gen_chunks() {
    if (!initalized) {
        return;
    }
    
    log_info("Worldgen init...");

    i32 count = 0;
    f32 xoffset = rand();
    f32 zoffset = rand();
    for (i32 yy = 0; yy < WORLD_SIZE_Y; yy++) {
        for (i32 zz = 0; zz < WORLD_SIZE_Z; zz++) {
            for (i32 xx = 0; xx < WORLD_SIZE_X; xx++) {
                chunk *c = &state.chunks[_3D_TO_1D(xx, yy, zz, WORLD_SIZE_Z, WORLD_SIZE_X)];
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

                            if (sampley <= get_height(samplex, samplez, 65.0f, 4, 0.7f, 1.80f)) {
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
    for (i32 yy = 0; yy < WORLD_SIZE_Y; yy++) {
        for (i32 zz = 0; zz < WORLD_SIZE_Z; zz++) {
            for (i32 xx = 0; xx < WORLD_SIZE_X; xx++) {
                log_info("Generating chunk mesh #%i", count);
                count ++;
                chunk *c = &state.chunks[_3D_TO_1D(xx, yy, zz, WORLD_SIZE_Z, WORLD_SIZE_X)];
                chunk *neighbouring[6];
                neighbouring[UP] = yy < (CHUNK_SIZE_Y - 1) ? &state.chunks[_3D_TO_1D(xx, yy+1, zz, WORLD_SIZE_Z, WORLD_SIZE_X)] : NULL;
                neighbouring[DOWN] = yy > 0 ? &state.chunks[_3D_TO_1D(xx, yy-1, zz, WORLD_SIZE_Z, WORLD_SIZE_X)] : NULL;
                neighbouring[RIGHT] = xx < (CHUNK_SIZE_X - 1) ? &state.chunks[_3D_TO_1D(xx+1, yy, zz, WORLD_SIZE_Z, WORLD_SIZE_X)] : NULL;
                neighbouring[LEFT] = xx > 0 ? &state.chunks[_3D_TO_1D(xx-1, yy, zz, WORLD_SIZE_Z, WORLD_SIZE_X)] : NULL;
                neighbouring[FRONT] = zz < (CHUNK_SIZE_Z - 1) ? &state.chunks[_3D_TO_1D(xx, yy, zz+1, WORLD_SIZE_Z, WORLD_SIZE_X)] : NULL;
                neighbouring[BACK] = zz > 0 ? &state.chunks[_3D_TO_1D(xx, yy, zz-1, WORLD_SIZE_Z, WORLD_SIZE_X)] : NULL;

                chunk_gen_mesh(c, &state.meshes[_3D_TO_1D(xx, yy, zz, WORLD_SIZE_Z, WORLD_SIZE_X)], neighbouring);
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
