#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project2.h"


void negative(struct rgb *col)
{
    /* Negate the inputted color struct (in place) */

    if (col == NULL) {
        fprintf(stderr,"NULL color\n");
        exit(1);
    }

    col->r = 255 - col->r;
    col->g = 255 - col->g;
    col->b = 255 - col->b;
}

void make_negative(struct image *img)
{
    /* Make an image into its negative. The image is modified in place */

    if (img == NULL) {
        fprintf(stderr,"NULL image\n");
        exit(1);
    }

    for (int i = 0; i < img->w * img->h; i++) {
        negative(&img->pixels[i]);
    } 
} 


int main(int argc, char *argv[])
{
    fprintf(stderr,"Converting PZ into negative\n");

    char *description = NULL; // out parameter for image description
    struct image *img = read_pz(&description);        

    char *new_descript = concatenate(description, " (negative)");
    make_negative(img);
    write_pz(img,new_descript);
 
    // Free variables
    free(description);
    free(new_descript);
    img_free(img);
    return 0;
}
