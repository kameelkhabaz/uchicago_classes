#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project2.h"

// read in a P6 file
struct image *read_p6()
{
    fprintf(stderr,"Starting reading P6 image\n");
    
    uint32_t w, h;
    read_p3_p6_header(stdin, &w, &h, "P6");

    struct rgb *pixels = read_byte_pixels(w * h);

    return img_new(w, h, pixels);
}

int main(int argc, char *argv[])
{
    fprintf(stderr,"Testing p6 to p3 conversion\n");
    struct image *img = read_p6();        

    write_p3(img);
    img_free(img);
    return 0;
}
