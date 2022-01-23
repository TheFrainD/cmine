#ifndef CMINE_CAMERA_H_
#define CMINE_CAMERA_H_

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "config.h"

#define PI GLM_PI
#define PI_2 GLM_PI_2
#define TAU (2 * PI)

typedef struct {
	mat4s view, proj;
	vec3s position, front, up, right, world_up;
	f32 yaw, pitch, fov, aspect;
} camera;

void camera_create(camera *self, f32 fov);

void camera_update(camera *self);

#endif // CMINE_CAMERA_H_
