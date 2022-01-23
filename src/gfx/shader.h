#ifndef CMINE_SHADER_H_
#define CMINE_SHADER_H_

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "config.h"

typedef struct {
	u32 id;
} shader;

void shader_create(shader *self, const char *vertex_path, const char *fragment_path);
void shader_destroy(shader *self);

void shader_use(shader *self);

void shader_f32(shader *self, const char *name, f32 value);
void shader_f32v(shader *self, const char *name, i32 count, const f32 *data);
void shader_vec2(shader *self, const char *name, vec2s vec); 
void shader_vec3(shader *self, const char *name, vec3s vec);
void shader_vec4(shader *self, const char *name, vec4s vec);
void shader_i32(shader *self, const char *name, i32 value);
void shader_i32v(shader *self, const char *name, i32 count, const i32 *data);
void shader_mat4(shader *self, const char *name, mat4s mat);

#endif // CMINE_SHADER_H_
