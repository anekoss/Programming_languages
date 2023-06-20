#include "bmp.h"
#include "file.h"
#include "image.h"
#include "rotation.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char** argv ) {
    if(argc < 3){
        printf("Мало аргументов");
	    return 1;
    }
    enum open_status open_status;
    enum close_status close_status;
    enum read_status read_status;
    enum write_status write_status;

    FILE* original_file = {0};
    FILE* new_file = {0};
    open_status = open_for_read(argv[1], &original_file);
    if(open_status != OPEN_OK){
        printf("Произошла ошибка при открытии");
	    return 1;
    }
    struct image image = {0};
    read_status = from_bmp(original_file, &image);
    if(read_status != READ_OK){
        printf("Произошла ошибка при чтении");
        close(original_file);
        image_delete(&image);
	    return 1;
    }
    close_status = close(original_file);
    if(close_status != CLOSE_OK){
        printf("Произошла ошибка при закрытии");
        image_delete(&image);
	    return 1;
    }
    
    open_status = open_for_write(argv[2], &new_file);
    if(open_status != OPEN_OK){
        printf("Произошла ошибка при открытии");
	    image_delete(&image);
	    return 1;
    }
    struct image n_image= {0};
    n_image = rotate(image);
    write_status =to_bmp(new_file, &n_image);
    if(write_status != WRITE_OK){
        printf("Произошла ошибка при записи");
	    close(new_file);
        image_delete(&n_image);
	    return 1;
    }
    close_status = close(new_file);
    if(close_status != CLOSE_OK){
        printf("Произошла ошибка при закрытии");
	    return 1;
    }
    image_delete(&image);
    image_delete(&n_image);
    return 0;
}


