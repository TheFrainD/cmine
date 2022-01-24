#ifndef CMINE_TEXTURE_H_
#define CMINE_TEXTURE_H_

#include "config.h" 

typedef struct {
    u32 id;
} texture;

void texture_create(texture *self, const char *path);
void texture_bind(texture *self, u16 unit);
void texture_destroy(texture *self);

#endif // CMINE_TEXTURE_H_
