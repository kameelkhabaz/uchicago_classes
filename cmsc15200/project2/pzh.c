#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project2.h"


void horizontal_flip(struct image *img)
{
    /* Horizontally flip an image. The image is modified in place */

    if (img == NULL) {
        fprintf(stderr,"NULL image\n");
        exit(1);
    }

    uint32_t width = img->w;
    uint32_t image_size = width * img->h;
    uint32_t x = 0; // current x position in image
    uint32_t start_r = 0; // index of start of current row

    for (int i = 0; i < image_size; i++) {
        if (x > width - 1) {
            // Reached next row in image grid, reset x to 0
            x = 0;
            start_r = i;
        }
        if (x < width/2) {
            // Flip pixels on left side of row, flip index x in row with
            // index of  width - x - 1 of same row

            struct rgb temp = img->pixels[start_r +  width - x - 1];
            img->pixels[start_r +  width - x - 1] = img->pixels[start_r + x];
            img->pixels[start_r + x] = temp;
        }
        x++;
    } 
} 


int main(int argc, char *argv[])
{
    fprintf(stderr,"Horizontally flipping a PZ image\n");

    char *description = NULL; // out parameter for image description
    struct image *img = read_pz(&description);        

    char *new_descript = concatenate(description, " (flipped horizontally)");
    horizontal_flip(img);
    write_pz(img,new_descript);
 
    // Free variables
    free(description);
    free(new_descript);
    img_free(img);
    return 0;
}
