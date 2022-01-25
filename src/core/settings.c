#include "settings.h"

#include <log.h>
#define LIBCONFIG_STATIC
#include <libconfig.h>

// Default settings
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define VSYNC 1

settings game_settings;

static b8 initialized = FALSE;

b8 settings_init() {
    if (initialized) {
        return FALSE;
    }

    log_trace("Loading settings...");

    config_t cfg;
	config_init(&cfg);
	config_read_file(&cfg, "../config/settings.cfg");

	if (!config_lookup_int(&cfg, "width", &game_settings.window_width) || !config_lookup_int(&cfg, "height", &game_settings.window_height)) {
		log_error("Could not read screen size in config file!");
		game_settings.window_width = WINDOW_WIDTH;
		game_settings.window_height = WINDOW_HEIGHT;
	}

	if (!config_lookup_int(&cfg, "vsync", &game_settings.vsync)) {
		log_error("Could not read vsync in config file!");
		game_settings.vsync = VSYNC;
	}

	config_destroy(&cfg);
    initialized = TRUE;

    return TRUE;
}