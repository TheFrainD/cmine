#include "texture.h"

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <log.h>

void texture_create(texture *self, const char *path) {
    i32 ch, width, height;
    u8 *buf = stbi_load(path, &width, &height, &ch, STBI_rgb_alpha);
    if (!buf) {
        log_error("Could not load texture at \"%s\"", path);
        return;
    }

    glGenTextures(1, &self->id);
    glBindTexture(GL_TEXTURE_2D, self->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(buf);
}

void texture_bind(texture *self, u16 unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, self->id);
}

void texture_destroy(texture *self) {
    glDeleteTextures(1, &self->id);
}