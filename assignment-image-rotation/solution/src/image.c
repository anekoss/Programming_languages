#include "image.h"

struct image image_create(const uint32_t width, const uint32_t height){
	struct image img = {0};
	img.height = height;
	img.width = width;
	img.data = malloc (sizeof(struct pixel)*width*height);
	if (img.data == NULL)
	{
		printf("Произошла ошибка при выделении памяти");

	}
	return img;
}



void image_delete(const struct image* image){
			free(image->data);
}
