#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project2.h"


int main(int argc, char *argv[])
{
    fprintf(stderr,"Converting p3 to pz\n");

    // Read in p3 from stdin, save as image struct, then write image 
    // struct to pz file to stdout.
    struct image *img = read_p3(stdin);        

    // Check for descriptor string
    if (argc > 1)
        write_pz(img,argv[1]);
    else
        write_pz(img,"[image, no description]");

    // Free img struct
    img_free(img);
    return 0;
}

