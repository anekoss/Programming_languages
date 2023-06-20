#include "rotation.h"
#include "image.h"

struct image rotate(const struct image source){
	const uint32_t width = source.width;
	const uint32_t height = source.height;
	const struct pixel* oldData = source.data;
	const struct image img = image_create(height, width);
	struct pixel* newData = img.data;
	for(size_t i = (width*height)-width; i < width*height; i++)
		for(size_t j = i; j < height*width; j-=width){
			*newData = oldData[j];
			newData += 1;
	}
	return img;
}

