#ifndef _IMAGE_H_
#define _IMAGE_H_
#include<inttypes.h>
#include<malloc.h>

struct pixel {
	uint8_t b;
	uint8_t g;
	uint8_t r;
};

struct image{
	uint64_t width;
	uint64_t height;
	struct pixel* data;
};

struct image image_create(const uint32_t width, const uint32_t height);

void image_delete(const struct image* image);
#endif

