#include "file.h"

enum open_status open_for_read(const char *pathname, FILE **file){
	if(!pathname)
		return OPEN_ERROR;
	FILE *f = fopen(pathname, "rb");
	if(!f){
  		return OPEN_ERROR;
	}
	*file = f;
	
	return OPEN_OK;

}

enum open_status open_for_write(const char* pathname, FILE **file){
	if(!pathname)
		return OPEN_ERROR;
	FILE *f = fopen(pathname, "wb");
	if(!f){
  		return OPEN_ERROR;
	}
	*file = f;
	return OPEN_OK;
}

enum close_status close(FILE *file){
	if(!file)
		return CLOSE_ERROR;
	int status = fclose(file);
	if(status != 0) return CLOSE_ERROR;
	return CLOSE_OK;
}
