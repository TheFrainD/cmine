#include "shader.h"

#include <stdlib.h>

#include <glad/glad.h>
#include <log.h>

#include "cglm/types-struct.h"
#include "utils/utils.h"

enum compile_type {
	SHADER_PROGRAM,
	VERTEX_SHADER = GL_VERTEX_SHADER,
	FRAGMENT_SHADER = GL_FRAGMENT_SHADER
};

static void gl_err() {
	i32 glerr = glGetError();
	while (glerr != GL_NO_ERROR) {
		log_error("OpenGL error: %i", glerr);
		glerr = glGetError();
	}
}

static void err_log(u32 handle, i32 type) {
	i32 len, ch;
	char *log;
	if (type == SHADER_PROGRAM) {
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &len);
	} else {
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &len);
	}
	if (len > 0) {
		log = malloc(len);
		if (type == SHADER_PROGRAM) {
			glGetProgramInfoLog(handle, len, &ch, log);
			log_error("Shader program log:\n%s", log);
		} else {
			glGetShaderInfoLog(handle, len, &ch, log);
			log_error("%s shader log:\n%s", type == VERTEX_SHADER ? "Vertex" : "Fragment",log);
		}
		free(log);
	}
}

static u32 shader_compile(const char *src, i32 type) {
	u32 shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, 0);
	i32 res;
	glCompileShader(shader);
	gl_err();
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (!res) {
		log_error("Could not compile %s shader!", type == VERTEX_SHADER ? "vertex" : "fragment");
		err_log(shader, type);
	}
	return shader;
}

static u32 program_link(u32 vertex, u32 fragment) {
	u32 prog = glCreateProgram();
	glAttachShader(prog, vertex);
	glAttachShader(prog, fragment);
	i32 res;
	glLinkProgram(prog);
	gl_err();
	glGetProgramiv(prog, GL_LINK_STATUS, &res);
	if (!res) {
		log_error("Could not link shader program!");
		err_log(prog, SHADER_PROGRAM);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return prog;
}

void shader_create(shader *self, const char *vertex_path, const char *fragment_path) {
	u32 vertex = shader_compile(utils_load_text(vertex_path), VERTEX_SHADER);
	u32 fragment = shader_compile(utils_load_text(fragment_path), FRAGMENT_SHADER);
	self->id = program_link(vertex, fragment);
}
void shader_destroy(shader *self) {
	glDeleteProgram(self->id);
}

void shader_use(shader *self) {
	glUseProgram(self->id);
}

void shader_f32(shader *self, const char *name, f32 value) {
	shader_use(self);
	glUniform1f(glGetUniformLocation(self->id, name), value);
}

void shader_f32v(shader *self, const char *name, i32 count, const f32 *data) {
	shader_use(self);
	glUniform1fv(glGetUniformLocation(self->id, name), count, data);
}

void shader_vec2(shader *self, const char *name, vec2s vec) {
	shader_use(self);
	glUniform2f(glGetUniformLocation(self->id, name), vec.x, vec.y);
}

void shader_vec3(shader *self, const char *name, vec3s vec) {
	shader_use(self);
	glUniform3f(glGetUniformLocation(self->id, name), vec.x, vec.y, vec.z);
}

void shader_vec4(shader *self, const char *name, vec4s vec) {
	shader_use(self);
	glUniform4f(glGetUniformLocation(self->id, name), vec.x, vec.y, vec.z, vec.w);
}

void shader_i32(shader *self, const char *name, i32 value) {
	shader_use(self);
	glUniform1i(glGetUniformLocation(self->id, name), value);
}

void shader_i32v(shader *self, const char *name, i32 count, const i32 *data) {
	shader_use(self);
	glUniform1iv(glGetUniformLocation(self->id, name), count, data);
}

void shader_mat4(shader *self, const char *name, mat4s mat) {
	shader_use(self);
	glUniformMatrix4fv(glGetUniformLocation(self->id, name), 1, GL_FALSE, (const GLfloat *)mat.raw);
}
