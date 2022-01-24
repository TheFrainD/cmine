#include "window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <log.h>

#include "event.h"

typedef struct {
	GLFWwindow *window;
	i32 width;
	i32 height;
	char *title;
} window_state;

static b8 initialized = FALSE;
static window_state state;

static void error_callback(i32 id, const char *msg) {
	log_error("GLFW error: %s", msg);
}

static void size_callback(GLFWwindow *window, i32 width, i32 height) {
	state.width = width;
	state.height = height;
	glViewport(0, 0, width, height);

	event_context context;
	context.data.i32[0] = width;
	context.data.i32[1] = height;
	event_publish(EVENT_CODE_WINDOW_RESIZED, &state, context);
}

b8 window_create(i32 width, i32 height, char *title) {
	if (initialized) {
		return FALSE;
	}

	state.width = width;
	state.height = height;
	state.title = title;
	
	// Init GLFW
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
		log_fatal("Could not initialize GLFW!");
		return FALSE;
	}
	log_trace("GLFW initialized");
	
	// Configure OpenGL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// set OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);	// to make MacOS happy
	
	// Create window
	state.window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!state.window) {
		log_fatal("Could not create window!");
		glfwTerminate();
		return FALSE;
	}
	log_trace("Window created");

	glfwMakeContextCurrent(state.window);
	glfwSwapInterval(1);	// V-Sync

	glfwSetWindowSizeCallback(state.window, size_callback);

	// Init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		log_fatal("Could not initialize glad!");
		glfwTerminate();
		return FALSE;
	}
	log_trace("glad initialized");

	glViewport(0, 0, width, height);

	// Print system information
	log_info("Vendor: %s", glGetString(GL_VENDOR));
	log_info("Renderer: %s", glGetString(GL_RENDERER));
	i32 major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	log_info("OpenGL version: %i.%i", major, minor);
	
	initialized = TRUE;
	return TRUE;
}

void window_terminate() {
	if (!initialized) {
		return;
	}

	glfwDestroyWindow(state.window);
	glfwTerminate();
	log_trace("Window subsystem terminated");
}

b8 window_poll_events() {
	if (!initialized) {
		return FALSE;
	}

	glfwPollEvents();
	return !glfwWindowShouldClose(state.window);
}

void window_close() {
	glfwSetWindowShouldClose(state.window, GLFW_TRUE);
}

void window_swap_buffers() {
	if (!initialized) {
		return;
	}

	glfwSwapBuffers(state.window);
}

void window_key_callback(void *callback) {
	if (!initialized) {
		return;
	}

	glfwSetKeyCallback(state.window, callback);
}

void window_mouse_callback(void *callback) {
	if (!initialized) {
		return;
	}

	glfwSetMouseButtonCallback(state.window, callback);
}

void window_cursor_callback(void *callback) {
	if (!initialized) {
		return;
	}

	glfwSetCursorPosCallback(state.window, callback);
}

i32 window_width() {
	if (!initialized) {
		return 0;
	}

	return state.width;
}

i32 window_height() {
	if (initialized) {
		return 0;
	}

	return state.height;
}

void window_set_key_callback(void *callback) {
	glfwSetKeyCallback(state.window, callback);
}

void window_set_mouse_callback(void *callback) {
	glfwSetMouseButtonCallback(state.window, callback);
}

void window_set_cursor_pos_callback(void *callback) {
	glfwSetCursorPosCallback(state.window, callback);
}	

void window_mouse_grab(b8 grab) {
	glfwSetInputMode(state.window, GLFW_CURSOR, grab ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}
