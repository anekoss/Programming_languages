#ifndef _FILEOC_
#define _FILEOC_
#include <stdio.h>
#include <stddef.h>

enum open_status {
	OPEN_OK = 0,
	OPEN_ERROR
};

enum open_status open_for_read(const char *pathname, FILE **file);

enum open_status open_for_write(const char *pathname, FILE **file);

enum close_status {
	CLOSE_OK = 0,
	CLOSE_ERROR
};

enum close_status close(FILE *file);

#endif
