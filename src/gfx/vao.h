#ifndef CMINE_VAO_H_
#define CMINE_VAO_H_

#include "config.h"
#include "vbo.h"

typedef struct {
	u32 arr;
} vao;

void vao_create(vao *self);
void vao_destroy(vao *self);
void vao_bind(vao *self);
void vao_attrib(vao *self, vbo *vbo, u32 index,
		i32 size, i32 type, i32 stride, i32 offset);

#endif // CMINE_VAO_H_
