#ifndef CMINE_GAME_H_
#define CMINE_GAME_H_

#include "config.h"

typedef b8 (*game_init)();
typedef b8 (*game_update)(f64 delta_time);
typedef b8 (*game_render)(f64 delta_time);
typedef b8 (*game_destroy)();

b8 game_create(game_init init, game_update update,
		game_render render, game_destroy destroy);

b8 game_run();
void game_shutdown();

i32 get_window_width();
i32 get_window_height();

f64 game_delta_time();

#endif // CMINE_GAME_H_
