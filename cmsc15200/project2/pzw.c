#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "project2.h"


struct image *widen(struct image *img)
{
    /* Widen an image by a factor of 2, leaving the height alone. 
     * Return a new image */

    if (img == NULL) {
        fprintf(stderr,"NULL image\n");
        exit(1);
    }

    uint32_t n_pixels = img->w * img->h;
    
    // pixels of widened image
    struct rgb *w_pixels = pixels_new(n_pixels * 2);

    // Repeat each pixel twice to widen the image
    for (int i = 0; i < n_pixels; i++) {
        w_pixels[2*i] = img->pixels[i];
        w_pixels[2*i + 1] = img->pixels[i];
    }
    
    return img_new(img->w * 2, img->h, w_pixels);
} 


int main(int argc, char *argv[])
{
    fprintf(stderr,"Widening a PZ image by a factor of 2\n");

    char *description = NULL; // out parameter for image description
    struct image *img = read_pz(&description);        

    char *new_descript = concatenate(description, " (widened*2)");
    struct image *wide_image = widen(img);
    img_free(img);

    write_pz(wide_image,new_descript);
 
    // Free variables
    free(description);
    free(new_descript);
    img_free(wide_image);
    return 0;
}
