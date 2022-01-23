#include "utils.h"

#include <stdlib.h>
#include <stdio.h>

#include <log.h>

char *utils_load_text(const char *path) {
	// Open file
	FILE *fp = fopen(path, "rb");
	if (!fp) {
		log_error("Could not load file at \"%s\"", path);
		return NULL;
	}
	
	// Calculate length
	fseek(fp, 0, SEEK_END);
	u64 len = ftell(fp);
	rewind(fp);
	
	// Allocate buffer
	char *buf = calloc(1, len+1);
	if (!buf) {
		log_error("Could not allocate buffer for file at \"%s\"", path);
		fclose(fp);
		return NULL;
	}

	// Read file's contents to buffer
	fread(buf, 1, len, fp);
	fclose(fp);

	return buf;
}
