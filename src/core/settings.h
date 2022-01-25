#ifndef CMINE_SETTINGS_H_
#define CMINE_SETTINGS_H_

#include "config.h"

typedef struct {
	i32 window_width;
	i32 window_height;
	i32 vsync;
} settings;

b8 settings_init();

extern settings game_settings;

#endif // CMINE_SETTINGS_H_