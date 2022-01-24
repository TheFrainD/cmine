#ifndef CMINE_WINDOW_H_
#define CMINE_WINDOW_H_

#include "config.h"

b8 window_create(i32 width, i32 height, char *title);
void window_terminate();

b8 window_poll_events();
void window_close();
void window_swap_buffers();

i32 window_width();
i32 window_height();

void window_set_key_callback(void *);
void window_set_mouse_callback(void *);
void window_set_cursor_pos_callback(void *);

void window_mouse_grab(b8 grab);

#endif // CMINE_WINDOW_H_
