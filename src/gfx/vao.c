#include "vao.h"
#include "gfx/vbo.h"

#include <glad/glad.h>

void vao_create(vao *self) {
	glGenVertexArrays(1, &self->arr);
}

void vao_destroy(vao *self) {
	glDeleteVertexArrays(1, &self->arr);
}

void vao_bind(vao *self) {
	glBindVertexArray(self->arr);
}

void vao_attrib(vao *self, vbo *vbo, u32 index,
	i32 size, i32 type, i32 stride, i32 offset) {
	vao_bind(self);
	vbo_bind(vbo);
	glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void *)offset);
	glEnableVertexAttribArray(index);
}
