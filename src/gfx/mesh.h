#ifndef CMINE_MESH_H_
#define CMINE_MESH_H_

#include "config.h"
#include "gfx/vao.h"
#include "gfx/vbo.h"

typedef struct {
    vbo vbo_;
    vao vao_;
    u32 size;
    b8 used;
} mesh;

void mesh_create(mesh *self, f32* buffer, u32 size, const int *attrib, b8 used);
void mesh_destroy(mesh *self);
void mesh_draw(mesh *self);

#endif // CMINE_MESH_H_