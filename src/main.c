#include <glad/glad.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>
#include <log.h>

#include "cglm/struct/vec3.h"
#include "config.h"
#include "core/event.h"
#include "core/game.h"
#include "core/input.h"
#include "gfx/shader.h"
#include "gfx/vao.h"
#include "gfx/vbo.h"
#include "utils/camera.h"

f32 vertices[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

i32 state = 1;

shader shader_;
vao vao_;
vbo vbo_;

mat4s model;
vec3s cube_pos;

camera camera_;

b8 key_handler(u16 code, void *sender, void *listener, event_context context) {
    if (context.data.u16[0] == KEY_ESCAPE) {
		game_shutdown();
	}

    return TRUE;
}

b8 init() {
	vao_create(&vao_);
	vbo_create(&vbo_, GL_ARRAY_BUFFER, FALSE);
	vbo_data(&vbo_, sizeof(vertices), vertices);
	vao_attrib(&vao_, &vbo_, 0, 3, GL_FLOAT, sizeof(float) * 3, 0);

	shader_create(&shader_, "res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    
    camera_create(&camera_, 1.0472f);

    cube_pos.y = -2.0f;
    
    b8 suc = event_subscribe(EVENT_CODE_KEY_PRESSED, &state, key_handler); 

	return TRUE;
}

b8 update(f64 delta_time) {	
    
    f32 velocity = 10.0f * delta_time;
    if (input_key_pressed(KEY_W)) {
        camera_.position = glms_vec3_add(camera_.position, glms_vec3_scale(camera_.front, velocity));
    } else if (input_key_pressed(KEY_S)) {
        camera_.position = glms_vec3_sub(camera_.position, glms_vec3_scale(camera_.front, velocity));
    }

    if (input_key_pressed(KEY_A)) {
        camera_.position = glms_vec3_sub(camera_.position, glms_vec3_scale(camera_.right, velocity));
    } else if (input_key_pressed(KEY_D)) {
        camera_.position = glms_vec3_add(camera_.position, glms_vec3_scale(camera_.right, velocity));
    } 
    
    camera_update(&camera_);

    return TRUE;
}

b8 render(f64 delta_time) {
	glClearColor(0.0, 0.2, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_use(&shader_);
   
    model = glms_translate_make(cube_pos);

    shader_mat4(&shader_, "model", model);
    shader_mat4(&shader_, "view", camera_.view);
    shader_mat4(&shader_, "proj", camera_.proj);

	vao_bind(&vao_);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	return TRUE;
}

b8 destroy() {
	return TRUE;
}

int main(void) {
	game_create(init, update, render, destroy);
	game_run();
	return 0;
}
