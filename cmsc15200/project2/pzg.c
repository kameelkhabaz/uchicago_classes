#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project2.h"


double luminance(struct rgb col)
{
    /* Calculate luminance as 0.2126r + 0.7152g + 0.0722b */

    return 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
}

void make_grayscale(struct image *img)
{
    /* Make an image into grayscale. The image is modified in place */

    if (img == NULL) {
        fprintf(stderr,"NULL image\n");
        exit(1);
    }

    for (int i = 0; i < img->w * img->h; i++) {
        unsigned char lum = luminance(img->pixels[i]); 
        // truncated luminance
        img->pixels[i].r = lum;
        img->pixels[i].g = lum;
        img->pixels[i].b = lum;
    } 
} 


int main(int argc, char *argv[])
{
    fprintf(stderr,"Converting PZ into grayscale\n");

    char *description = NULL; // out parameter for image description
    struct image *img = read_pz(&description);        

    char *new_descript = concatenate(description, " (grayscale)");
    make_grayscale(img);
    write_pz(img,new_descript);
 
    // Free variables
    free(description);
    free(new_descript);
    img_free(img);
    return 0;
}
