#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project2.h"



int main(int argc, char *argv[])
{
    fprintf(stderr,"Converting pz to p3\n");

    // Read in pz from stdin, save as image struct, then write image 
    // struct to p3 file to stdout.

    struct image *img = read_pz(NULL);        
    write_p3(img);

    // Free struct
    img_free(img);
    return 0;
}
