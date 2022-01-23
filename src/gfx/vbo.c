#include "vbo.h"

#include <glad/glad.h>

void vbo_create(vbo *self, i32 type, b8 dynamic) {
	glGenBuffers(1, &self->buf);
	self->type = type;
	self->dynamic = dynamic;
}

void vbo_destroy(vbo *self) {
	glDeleteBuffers(1, &self->buf);
}

void vbo_bind(vbo *self) {
	glBindBuffer(self->type, self->buf);
}

void vbo_data(vbo *self, i32 size, void *data) {
	vbo_bind(self);
	glBufferData(self->type, size, data, self->dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
