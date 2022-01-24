#include "input.h"

#include <string.h>

#include <GLFW/glfw3.h>
#include <log.h>

#include "window.h"
#include "event.h"

typedef struct {
	b8 keys[KEYS_MAX_KEYS];
} keyboard_state;

typedef struct {
	b8 grabbed;
	b8 buttons[MOUSE_BUTTON_MAX];
	f64 x, y;
	f64 prev_x, prev_y;
} mouse_state;

typedef struct {
	keyboard_state keyboard;
	mouse_state mouse;
} input_state;

static b8 initialized = FALSE;
static input_state state;

static void key_callback(GLFWwindow *window, i32 key, i32 scancode, i32 action, i32 mode) {
	state.keyboard.keys[key] = action;

	event_context context;
	context.data.u16[0] = key;
	b8 suc = event_publish(EVENT_CODE_KEY_PRESSED, NULL, context);
}

static void mouse_callback(GLFWwindow *window, i32 button, i32 action, i32 mode) {
	if (state.mouse.buttons[button] != action) {
		state.mouse.buttons[button] = action;

		event_context context;
		context.data.u16[0] = button;
		event_publish(action ? EVENT_CODE_MOUSE_BUTTON_PRESSED : EVENT_CODE_KEY_RELEASED, &state, context);
	}
}

void cursor_callback(GLFWwindow *window, f64 x, f64 y) {
	state.mouse.prev_x = state.mouse.x;
	state.mouse.prev_y = state.mouse.y;
	state.mouse.x = x;
	state.mouse.y = y;

	event_context context;
	context.data.f32[0] = x;
	context.data.f32[1] = y;
	context.data.f32[2] = state.mouse.prev_x;
	context.data.f32[3] = state.mouse.prev_y;
	event_publish(EVENT_CODE_MOUSE_MOVED, &state, context);
}

b8 input_init() {
	if (initialized) {
		return FALSE;
	}

	memset(state.keyboard.keys, 0, sizeof(state.keyboard.keys));
	memset(state.mouse.buttons, 0, sizeof(state.mouse.buttons));

	window_set_key_callback(key_callback);
	window_set_mouse_callback(mouse_callback);
	window_set_cursor_pos_callback(cursor_callback);

	state.mouse.grabbed = TRUE;
	window_mouse_grab(state.mouse.grabbed);

	initialized = TRUE;
	log_trace("Input subsystem initialized");

	return TRUE;
}

void input_terminate() {
	initialized = FALSE;
	log_trace("Input subsystem terminated");
}

b8 input_key_pressed(u16 key) {
	if (!initialized) {
		return FALSE;
	}

	return state.keyboard.keys[key];
}

b8 input_key_released(u16 key) {
	if (!initialized) {
		return FALSE;
	}

	return !state.keyboard.keys[key];
}

b8 input_mbutton_pressed(u16 button) {
	if (!initialized) {
		return FALSE;
	}

	return state.mouse.buttons[button];
}

b8 input_mbutton_released(u16 button) {
	if (!initialized) {
		return FALSE;
	}

	return state.mouse.buttons[button];
}

void input_mouse_pos(f64 *x, f64 *y) {
	if (!initialized) {
		*x = 0.0;
		*y = 0.0;
		return;
	}

	*x = state.mouse.x;
	*y = state.mouse.y;
}

void input_mouse_prev(f64 *x, f64 *y) {
	if (!initialized) {
		*x = 0.0;
		*y = 0.0;
		return;
	}
	
	*x = state.mouse.prev_x;
	*y = state.mouse.prev_y;
}

b8 input_is_mouse_grabbed() {
	return state.mouse.grabbed;
}

void input_set_mouse_grabbed(b8 grabbed) {
	state.mouse.grabbed = grabbed;
	window_mouse_grab(grabbed);
}