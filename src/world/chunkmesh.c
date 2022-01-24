#include "chunkmesh.h"

#define VERTEX_SIZE (3 + 2 + 1)

#define GET_BLOCK(x, y, z) (chunk->blocks[((y) * CHUNK_SIZE_Z + (z)) * CHUNK_SIZE_X + (x)])
#define IS_IN(x, y, z) ((x) >= 0 && (x) < CHUNK_SIZE_X && (y) >= 0 && (y) < CHUNK_SIZE_Y && (z) >= 0 && (z) < CHUNK_SIZE_Z)
#define IS_BLOCKED(x, y, z) ((IS_IN(x, y, z)) && GET_BLOCK(x, y, z))
#define VERTEX_APPEND(i, x, y, z, u, v, l) buffer[i+0] = (x);\
                                        buffer[i+1] = (y);\
                                        buffer[i+2] = (z);\
                                        buffer[i+3] = (u);\
                                        buffer[i+4] = (v);\
                                        buffer[i+5] = (l);\
                                        i += VERTEX_SIZE;

const i32 attrib[] = {3, 2, 1, 0};

void chunk_gen_mesh(chunk *chunk, mesh *mesh) {
    f32 *buffer = (f32 *)malloc(sizeof(f32) * CHUNK_VOLUME * VERTEX_SIZE * 36);
    i32 index = 0;

    for (i32 y = 0; y < CHUNK_SIZE_Y; y++) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; z++) {
            for (i32 x = 0; x < CHUNK_SIZE_X; x++) {
                block b = GET_BLOCK(x, y, z);

                if (!b) {
                    continue; // Air block
                }

                f32 l;

                f32 uvsize = 1.0f / 16.0f;
                f32 u = ((b-1) % 16) * uvsize;
                f32 v = ((b-1) / 16) * uvsize;

                if (!IS_BLOCKED(x, y+1, z) ) {
                   l = 1.0f;
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v, l);
				}
				if (!IS_BLOCKED(x, y-1, z)) {
					l = 0.65f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}

				if (!IS_BLOCKED(x+1, y, z)) {
					l = 0.92f;
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u,v, l);
				}

				if (!IS_BLOCKED(x-1, y, z)) {
					l = 0.81f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}

				if (!IS_BLOCKED(x, y, z+1)) {
					l = 0.88f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
				}

				if (!IS_BLOCKED(x, y, z-1)) {
					l = 0.72f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u,v, l);
				}
            }
        }
    }

    mesh_create(mesh, buffer, CHUNK_VOLUME * 36, attrib);
}