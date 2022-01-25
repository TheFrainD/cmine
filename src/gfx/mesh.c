#include "mesh.h"

#include <glad/glad.h>
#include <log.h>

void mesh_create(mesh *self, f32* buffer, u32 size, const int *attrib, b8 used) {
    self->used = used;
    self->size = size;
    
    if (used) {
        i32 vertex_size = 0;
        for (i32 i = 0; attrib[i]; i++) {
            vertex_size += attrib[i];
        } 

        vao_create(&self->vao_);
        vbo_create(&self->vbo_, GL_ARRAY_BUFFER, FALSE);

        vbo_data(&self->vbo_, sizeof(f32) * vertex_size * size, buffer);
        
        i32 offset = 0;
        for (i32 i = 0; attrib[i]; i++) {
            vao_attrib(&self->vao_, &self->vbo_, i, attrib[i], GL_FLOAT, vertex_size * sizeof(f32), offset * sizeof(f32));
            offset += attrib[i];
        }
    }
}

void mesh_destroy(mesh *self) {
    vao_destroy(&self->vao_);
    vbo_destroy(&self->vbo_);
}

void mesh_draw(mesh *self) {
    vao_bind(&self->vao_);
    glDrawArrays(GL_TRIANGLES, 0, self->size);
}
