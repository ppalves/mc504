#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint8_t * convoluction(uint8_t *image, int width, int height, int[3][3] kernel) {
    

}

int offset(int i, int j, int width) {
    return i * width + j;
}

int main(int argc, char const *argv[]) {
    int width, height, bpp;

    uint8_t* rgb_image = stbi_load("assets/baboon.png", &width, &height, &bpp, 3);

    
    for (int i = 0; i < width*height; i++) {
        printf("%d\n", rgb_image[i]);
    }


    

    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, bpp);
    
    stbi_image_free(rgb_image);
    
    return 0;
}
