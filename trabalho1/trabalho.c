#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct convoluction_args {
    uint8_t *image;
    int **kernel;
    uint8_t *target;
    int width;
    int height; 
    float D;
    int start;
    int end;
} convoluction_args;


int offset(int i, int j, int width) {
    return i*width + j;
}

uint8_t multiple_kernel(int i, int j, uint8_t *image, int kernel[3][3], int width, float D) {
    int acc = 0;
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++){
            acc = acc + (kernel[x+1][y+1] * image[offset(i+x, j+y, width)]);
        }   
    }
    return (uint8_t) (D * acc);
}


void* convoluction(void *arguments) {
    convoluction_args *args = (convoluction_args*) arguments;
    for (int i = args->start; i < args->end; i++) {
        for (int j = 1; j < args->width -1; j++) {
            args->target[offset(i, j, args->width)] = multiple_kernel(i, j, args->image, args->kernel, args->width, args->D);
        }
    }
}


int main(int argc, char const *argv[]) {
    int width, height, channels;
    pthread_t thread[4];

    uint8_t *image = stbi_load("assets/baboon.png", &width, &height, &channels, 0);
    
    int kernel[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    float D = 1.0/9;


    uint8_t* processed_image = malloc(sizeof(uint8_t) * width * height * channels);

    int count = 0;
    for (int i = 1; i < 511; i = i + 128) {
        printf("%d, %d\n", i, i+128);
        convoluction_args *args = malloc(sizeof(convoluction_args));
        args->image = image;
        args->kernel = kernel;
        args->target = processed_image;
        args->width = width;
        args->height = height;
        args->D = D;
        args->start = i;
        args->end = i + 128;
        pthread_create(&thread[count], NULL, convoluction, (void *)args);
        count++;
    }
    
    for (int i = 0; i < 4; i++) 
        pthread_join(thread[i], NULL); 

    stbi_write_png("output.png", width, height, channels, processed_image, width*channels);
        
    stbi_image_free(image);
    free(processed_image);

    return 0;
}
