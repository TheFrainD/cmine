#include "camera.h"

#include <math.h>

#include <log.h>

#include "cglm/struct/affine.h"
#include "core/game.h"
#include "core/event.h"
#include "core/window.h"

#define MOUSE_SENSITIVITY 0.1f

void camera_update(camera *self) {
	self->pitch = CLAMP(self->pitch, -PI_2 + 0.01f, PI_2 - 0.01f);
	self->yaw = (self->yaw < 0 ? TAU : 0.0f) + fmodf(self->yaw, TAU);

	vec3s front = (vec3s){
		cosf(self->yaw) * cosf(self->pitch),
		sinf(self->pitch),
		sinf(self->yaw) * cosf(self->pitch) };
	
	self->front = glms_vec3_normalize(front);
	self->right = glms_vec3_normalize(glms_vec3_cross(self->front, self->world_up));
	self->up = glms_vec3_normalize(glms_vec3_cross(self->right, self->front));

	self->proj = glms_perspective(self->fov, (f32)get_window_width() / (f32)get_window_height(), 0.1f, 1000.0f);
	self->view = glms_lookat(self->position, glms_vec3_add(self->position, self->front), self->up);
}

static b8 cam_proj_callback(u16 code, void *sender, void *listener, event_context context) {
	camera *self = listener;
	camera_update(self);
	return TRUE;
}

static b8 cam_look_update(u16 code, void *sender, void *listener, event_context context) {
	camera *self = listener;
	
	f64 delta_x = CLAMP(context.data.f32[0] - context.data.f32[2], -100.0f, 100.0f);
	f64 delta_y = CLAMP(context.data.f32[3] - context.data.f32[1], -100.0f, 100.0f);
	self->yaw += delta_x * game_delta_time();
	self->pitch += delta_y * game_delta_time();

	camera_update(self);
	return TRUE;
}

void camera_create(camera *self, f32 fov) {
	self->position = GLMS_VEC3_ZERO;
	self->world_up = (vec3s){0.0f, 1.0f, 0.0f};
	self->front = (vec3s){0.0f, 0.0f, -1.0f};
	self->fov = fov;
	self->yaw = -PI_2;
	self->pitch = 0.0f;

	camera_update(self);	

	event_subscribe(EVENT_CODE_WINDOW_RESIZED, self, cam_proj_callback);
	event_subscribe(EVENT_CODE_MOUSE_MOVED, self, cam_look_update);
}
