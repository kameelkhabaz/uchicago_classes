#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// Write to a P6 file
void write_p6(struct image *img)    
{
    fprintf(stderr,"Starting writing P6 file\n");
    // Write to stdout
    printf("P6\n");
    printf("%d %d\n",img->w, img->h);
    
    // Max color value is 255 by default
    printf("255\n");
    
    if (img->w * img->h == 0) {
        fprintf(stderr,"Image is 0x0, finished writing P6 file.\n");
        return;
    }
 
    // Write the pixels 
    for (int i = 0; i < img->w * img->h; i++) {
        putc(img->pixels[i].r,stdout);
        putc(img->pixels[i].g,stdout);
        putc(img->pixels[i].b,stdout);
    }
    fprintf(stderr,"Finished writing P6 file\n");
}

int main(int argc, char *argv[])
{
    fprintf(stderr,"Converting P3 image to P6 image\n");
    struct image *img = read_p3();        
    img_show(stderr,img);
    write_p6(img);
    img_free(img);
    return 0;
}
