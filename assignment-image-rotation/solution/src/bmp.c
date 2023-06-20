#include "bmp.h"
#include "image.h"
#include <stdbool.h>
#define PADDINGS 3

static const struct bmp_header HEADER_TEMPLATE = {
    .bfType = 19778,
	.bfReserved = 0,
	.bOffBits = 54,
	.biSize = 40,
	.biPlanes = 1,
	.biBitCount = 24,
	.biCompression = 0,
	.biXPelsPerMeter = 2834,
	.biYPelsPerMeter = 2834,
	.biClrUsed = 0,
	.biClrImportant = 0
};

static bool check_type(struct bmp_header header){
	return header.bfType == 19778;
}


uint8_t get_padding(const uint32_t width) {
    const uint8_t mod = (width * sizeof(struct pixel)) % 4;
    return mod ? 4 - mod : mod;
}

static bool read_pixels(FILE *file,const uint32_t offset, const uint32_t width, const uint32_t height, struct pixel* data){
    const uint8_t padding = get_padding(width);
	fseek(file, offset, SEEK_SET);
	struct pixel* current = data;
	for(uint32_t i = 0; i < height; ++i){
		size_t read = fread(current, sizeof(struct pixel), width, file);
		if(!read){
			return false;}
		fseek(file, padding, SEEK_CUR);
		current+=read;
	}
	return true;
}
enum read_status from_bmp(FILE *in, struct image *img){
	if(!in){
		return (READ_INVALID_FILE);}
	if(!img){
		return READ_INVALID_CREATE;}
	struct bmp_header header = {0};
	size_t read = fread(&header, sizeof(struct bmp_header), 1, in);
	if(!read)
		return (READ_INVALID_HEADER);
	if(!check_type(header)){
		return (READ_INVALID_SIGNATURE);}
    const uint32_t width = header.biWidth;
	const uint32_t height = header.biHeight;
	const uint32_t offset = header.bOffBits;
	*img = image_create(width, height);
	if(!read_pixels(in, offset, width, height, img->data)){
		return (READ_INVALID_BITS);
	}
	return (READ_OK);
}


static bool build_header(const uint32_t width, const uint32_t height,
	const struct pixel* data, struct bmp_header *header){
	bool status = (!data || width == 0 || height == 0|| !header);
	if(status)
		return false;
	struct bmp_header new_header = HEADER_TEMPLATE;
	uint32_t sizeWithoutPadding = sizeof(struct pixel)*width*height;
	uint32_t sizeOfPadding = get_padding(width)*height;
	new_header.biSizeImage = sizeWithoutPadding + sizeOfPadding;
	new_header.bfileSize = sizeof(struct bmp_header) + new_header.biSizeImage;
	new_header.biWidth = width;
	new_header.biHeight = height;
	*header=new_header;
	return true;
}


static bool write_header(FILE *file,const struct bmp_header* header){
	return fwrite(header, sizeof(struct bmp_header), 1, file);
}

static bool write_pixels(FILE *file,uint32_t offset,const uint32_t width, const uint32_t height,
		const struct pixel* data){
	int answer = fseek(file, offset, SEEK_SET);
	if (answer == 0)
	{
		printf("     fseek");
	}
	const uint8_t padding = get_padding(width);
	const uint8_t zero[PADDINGS] = {0};
	for(uint32_t i = 0; i < height; ++i){
		size_t written = fwrite(data, sizeof(struct pixel), width, file);

		size_t size_to_write = fwrite(zero, 1, padding, file);
		if (size_to_write ==0 && padding != 0)
		{
			return false;
		}
		data+=written;
	}
	return true;
}

enum write_status to_bmp(FILE *out, const struct image *img){
	if(!img)
		return (WRITE_INVALID_CREATE);
	if(!out)
		return (WRITE_INVALID_FILE);
	const uint32_t height = img->height;
	const uint32_t width = img->width;
	struct pixel* data = img->data;
	struct bmp_header header = {0};
	if(!build_header(width, height, data, &header)) {
		return (WRITE_INVALID_IMAGE);
	}
	if(!write_header(out, &header)){
		return (WRITE_ERROR);
	}
	if(!write_pixels(out, header.bOffBits, width, height, data)){
		return (WRITE_ERROR);
	}
	return (WRITE_OK);
}
