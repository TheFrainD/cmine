#include "game.h"

#include <stdlib.h>
#include <stdio.h>

#include <log.h>
#define LIBCONFIG_STATIC
#include <libconfig.h>

#include "window.h"
#include "time.h"
#include "event.h"
#include "input.h"

#define WIDTH 800
#define HEIGHT 600
#define TITLE "Cmine"

typedef struct {
	b8 is_running;
	i32 width;
	i32 height;
	f64 last_time;
	f64 delta_time;
	
	game_init init;
	game_update update;
	game_render render;
	game_destroy destroy;
} game_state;

static b8 initilized = FALSE;
static game_state state;

static b8 window_resize(u16 code, void *sender, void *listener, event_context context) {
	state.width = context.data.i32[0];
	state.height = context.data.i32[1];

	return TRUE;
}

static b8 log_init() {
	FILE *fp = fopen("../log/cmine.log", "w+");
	if (!fp) {
		log_error("Could not open \"log/cmine.log\"!");
		return FALSE;
	}
	log_add_fp(fp, 0);
	return TRUE;
}

b8 game_create(game_init init, game_update update,
		game_render render, game_destroy destroy) {
	
	if (initilized) {
		return FALSE;
	}

	state.is_running = TRUE;
	state.init = init;
	state.update = update;
	state.render = render;
	state.destroy = destroy;

	log_init();
	log_trace("Cmine starts");

	config_t cfg;
	config_init(&cfg);
	config_read_file(&cfg, "../config/settings.cfg");

	if (!config_lookup_int(&cfg, "width", &state.width) || !config_lookup_int(&cfg, "height", &state.height)) {
		log_error("Could not read screen size in config file!");
		state.width = WIDTH;
		state.height = HEIGHT;
	}

	config_destroy(&cfg);

	if (!event_init()) {
		log_error("Could not start event subsystem!");
		return FALSE;
	}

	// Start window subsystem
	if (!window_create(state.width, state.height, TITLE)) {
		log_fatal("Could not start window subsystem!");
		return FALSE;
	}	

	event_subscribe(EVENT_CODE_WINDOW_RESIZED, &state, window_resize);

	if (!input_init()) {
		log_error("Could not start input subsystem!");
		return FALSE;
	}

	initilized = TRUE;
	return TRUE;
}

b8 game_run() {
	if (!initilized) {
		log_fatal("Game state is not initilized!");
		return FALSE;
	}

	state.init();

	while (state.is_running) {
		f64 current_time = time_now();
		state.delta_time = current_time - state.last_time;

		if (!window_poll_events()) {
			state.is_running = FALSE;
		}

		state.update(state.delta_time);

		// Render here
		state.render(state.delta_time);
		
		window_swap_buffers();

		state.last_time = current_time;
	}
	
	event_context context;
	event_publish(EVENT_CODE_GAME_SHUTDOWN, &state, context);

	// Terminate subsystems
	window_terminate();
	log_info("Cmine shuts down...");

	return TRUE;
}

void game_shutdown() {
	window_close();
}

i32 get_window_width() {
	return state.width;
}

i32 get_window_height() {
	return state.height;
}

f64 game_delta_time() {
	return state.delta_time;
}
