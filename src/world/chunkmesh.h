#ifndef CMINE_CHUNKMESH_H_
#define CMINE_CHUNKMESH_H_

#include "config.h"
#include "gfx/mesh.h"
#include "world/chunk.h"

enum faces {
	UP,
	DOWN,
    RIGHT,
	LEFT,
	FRONT,
	BACK,
};

void chunk_gen_mesh(chunk *self, mesh *mesh, chunk **neighbouring);

#endif // CMINE_CHUNKMESH_H_