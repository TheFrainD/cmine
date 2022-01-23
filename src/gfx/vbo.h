#ifndef CMINE_VBO_H_
#define CMINE_VBO_H_

#include "config.h"

typedef struct {
	u32 buf;
	i32 type;
	b8 dynamic;
} vbo;

void vbo_create(vbo *self, i32 type, b8 dynamic);
void vbo_destroy(vbo *self);
void vbo_bind(vbo *self);
void vbo_data(vbo *self, i32 size, void *data);

#endif // CMINE_VBO_H_
