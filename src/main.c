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
#include "gfx/texture.h"
#include "gfx/mesh.h"
#include "utils/camera.h"
#include "world/chunk.h"
#include "world/chunkmesh.h"
#include "world/worldgen.h"
#include "utils/math.h"

shader shader_;
texture terrain;
camera camera_;

f64 timer = 0.0;

b8 key_handler(u16 code, void *sender, void *listener, event_context context) {
    if (context.data.u16[0] == KEY_ESCAPE) {
		game_shutdown();
	}

    if (context.data.u16[0] == KEY_G) {
		if (timer == 0.0) {
            input_set_mouse_grabbed(!input_is_mouse_grabbed());
            timer = 1.0f;
        }
	}

    return TRUE;
}

b8 init() {

	shader_create(&shader_, "../res/shaders/chunk.vert", "../res/shaders/chunk.frag");
    texture_create(&terrain, "../res/textures/terrain.png");
    
    camera_create(&camera_, PI / 3.0f);
    camera_.position.z = 64.0f;
    camera_.position.y = 80.0f;
    camera_.position.z = 64.0f;

    world_init(10, 4, 10);
    world_gen_chunks();
    
    event_subscribe(EVENT_CODE_KEY_PRESSED, NULL, key_handler);
	return TRUE;
}

b8 update(f64 delta_time) {	
    
    f32 velocity = 15.0f * delta_time;
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

    if (timer > 0.0f) {
        timer -= delta_time;
        if (timer < 0.0) {
            timer = 0.0;
        }
    }

    return TRUE;
}

b8 render(f64 delta_time) {
	glClearColor(0.0, 0.709803f, 0.886274f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_use(&shader_);
   
    shader_mat4(&shader_, "view", camera_.view);
    shader_mat4(&shader_, "proj", camera_.proj);

    texture_bind(&terrain, 0);

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    world_draw(&shader_);

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
