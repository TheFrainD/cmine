#include "chunkmesh.h"

#include <cvec.h>

#include "utils/math.h"
#include "world/worldgen.h"

#define VERTEX_SIZE (3 + 2 + 1)

#define IS_IN_THIS(x, y, z) ((x) >= 0 && (x) < CHUNK_SIZE_X && (y) >= 0 && (y) < CHUNK_SIZE_Y && (z) >= 0 && (z) < CHUNK_SIZE_Z)
#define GET_CHUNK(x, y, z, dir) (IS_IN_THIS(x, y, z) ? self : neighbouring[dir])
#define IS_IN_CHUNK(x, y, z, dir) (GET_CHUNK(x, y, z, dir) != NULL)
#define COORD_BOUND(x, bound) (x >= bound ? (0) : (x >= 0 ? x : (bound-1)))
#define GET_BLOCK_FROM(x, y, z, dir) (IS_IN_CHUNK(x, y, z, dir) ? GET_CHUNK(x, y, z, dir)->blocks[(COORD_BOUND(y, CHUNK_SIZE_Y) * CHUNK_SIZE_Z + COORD_BOUND(z, CHUNK_SIZE_Z)) * CHUNK_SIZE_X + COORD_BOUND(x, CHUNK_SIZE_X)] : 0)
#define GET_BLOCK(x, y, z, dir) (IS_IN_THIS(x, y, z) ? (self->blocks[((y) * CHUNK_SIZE_Z + (z)) * CHUNK_SIZE_X + (x)]) : GET_BLOCK_FROM(x, y, z, dir))
#define IS_BLOCKED(x, y, z, dir) (!IS_IN_CHUNK(x, y, z, dir) || (GET_BLOCK(x, y, z, dir) != 0 && GET_BLOCK(x, y, z, dir) != 15))

#define VERTEX_APPEND(i, x, y, z, u, v, l) buffer[i+0] = (x);\
                                        buffer[i+1] = (y);\
                                        buffer[i+2] = (z);\
                                        buffer[i+3] = (u);\
                                        buffer[i+4] = (v);\
                                        buffer[i+5] = (l);\
                                        i += VERTEX_SIZE;

const i32 attrib[] = {3, 2, 1, 0};

void chunk_gen_mesh(chunk *self, mesh *mesh, chunk **neighbouring) {
    f32 *buffer = (f32 *)malloc(sizeof(f32) * CHUNK_VOLUME * VERTEX_SIZE * 36);
    i32 index = 0;

	b8 used = FALSE;

    for (i32 y = 0; y < CHUNK_SIZE_Y; y++) {
        for (i32 z = 0; z < CHUNK_SIZE_Z; z++) {
            for (i32 x = 0; x < CHUNK_SIZE_X; x++) {
                block b = GET_BLOCK(x, y, z, UP);

                if (!b) {
                    continue; // Air block
                }

                f32 l;

                f32 uvsize = 1.0f / 16.0f;
                f32 u = ((b-1) % 16) * uvsize;
                f32 v = ((b-1) / 16) * uvsize;

                f32 yoffset = 0.0f;
                if (b == 15) {
                    yoffset = -0.12f;
                }

                if (!IS_BLOCKED(x, y+1, z, UP)) {
                   	l = 1.0f;
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f + yoffset, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f + yoffset, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f + yoffset, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y + 0.5f + yoffset, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f + yoffset, z + 0.5f, u,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f + yoffset, z - 0.5f, u,v, l);
					used = TRUE;
				}

                if (b == 15) {
                    continue;
                }

				if (!IS_BLOCKED(x, y-1, z, DOWN)) {
					l = 0.65f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					used = TRUE;
				}

				if (!IS_BLOCKED(x+1, y, z, RIGHT)) {
					l = 0.92f;
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					used = TRUE;
				}

				if (!IS_BLOCKED(x-1, y, z, LEFT)) {
					l = 0.81f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					used = TRUE;
				}

				if (!IS_BLOCKED(x, y, z+1, FRONT)) {
					l = 0.88f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z + 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z + 0.5f, u,v, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z + 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z + 0.5f, u+uvsize,v+uvsize, l);
					used = TRUE;
				}

				if (!IS_BLOCKED(x, y, z-1, BACK)) {
					l = 0.72f;
					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x - 0.5f, y + 0.5f, z - 0.5f, u+uvsize,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);

					VERTEX_APPEND(index, x - 0.5f, y - 0.5f, z - 0.5f, u+uvsize,v, l);
					VERTEX_APPEND(index, x + 0.5f, y + 0.5f, z - 0.5f, u,v+uvsize, l);
					VERTEX_APPEND(index, x + 0.5f, y - 0.5f, z - 0.5f, u,v, l);
					used = TRUE;
				}
            }
        }
    }

	f32 *comp_buf = (f32 *)realloc(buffer, (index+1) * sizeof(f32));
	buffer = comp_buf;

    mesh_create(mesh, buffer, (index+1) / VERTEX_SIZE, attrib, used);
}