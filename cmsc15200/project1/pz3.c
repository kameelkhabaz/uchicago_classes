#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// Create heap-allocated array of 4 byte ints
uint32_t *arr_new(uint32_t len) 
{                 
    uint32_t *arr = (uint32_t *)malloc(sizeof(uint32_t) * len);
    if (arr == NULL) {
        fprintf(stderr,"arr: failed allocation\n");
        exit(1);
    }
    return arr;
}
       
// Read in grayscale pixels byte by byte
struct rgb *read_grayscale_pixels(uint32_t n_pixels)
{
    // Now read in pixels
    struct rgb *pixels = pixels_new(n_pixels);
    
    int i = 0;
    int col; // grayscale color value 
    while ((col = getc(stdin)) != EOF) {
        // Check image dimensions
        if (i > n_pixels - 1) {
            fprintf(stderr,"read_grayscale_pixels: Incorrect"
                           " image dimensions.\n");
            exit(1);
        }
        // Assign color
        pixels[i].r = col;
        pixels[i].g = col;
        pixels[i].b = col;
    
        fprintf(stderr,"Read in %d %d %d for pixel %d\n",pixels[i].r,
                pixels[i].g, pixels[i].b, i);
        i++;
    }
    
    return pixels;
}

// Read in a PZ file
struct image *read_pz()
{
    fprintf(stderr,"Starting reading in a PZ file\n");
    
    // Read in and check file type
    read_magic_number("PZ");
    
    // Bytes 2-9 won't be used
    for (int i = 2; i <= 9; i++) {
        getc(stdin);
    }

    // Get width and height of image
    unsigned char w1 = getc(stdin);
    unsigned char w2 = getc(stdin);
    uint32_t w = (w1 << 8) + w2;

    unsigned char h1 = getc(stdin);
    unsigned char h2 = getc(stdin);
    uint32_t h = (h1 << 8) + h2; 

    fprintf(stderr,"Image is %d x %d\n",w,h);

    // Bytes 14-17 won't be used
    for (int i = 14; i <= 17; i++) {
        getc(stdin);
    }

    unsigned char b18 = getc(stdin);
    // Check if image is grayscale
    unsigned char grayscale = b18 & 2;

    // Bytes 19-21 won't be used
    for (int i = 19; i <= 21; i++) {
        getc(stdin);
    }

    // Keep reading until reach null terminator
    char description[256] = {0};
    int i = 0;
    while ((description[i] = getc(stdin)) != '\0') 
        i++;
    fprintf(stderr,"Image description: %s\n",description);

    // Read # run lengths
    uint32_t n_runs = read_big_end_int();
    fprintf(stderr,"%d runs\n",n_runs);
    
    // Get run lengths 
    uint32_t *run_lengths = arr_new(n_runs);
        
    for (int i = 0; i < n_runs; i++) {
        // Read integer to the pointer of current array element
        run_lengths[i] = read_big_end_int();
        fprintf(stderr,"Run %d has length %d\n",i,run_lengths[i]);
    }
    
    uint32_t num_pixels = get_sum(run_lengths, n_runs);
 
    // Read in colors to get compresed pixels
    struct rgb *comp_pixels = NULL;
    if (grayscale) {
        fprintf(stderr,"Image is grayscale, extracting grayscale pixels\n");
        comp_pixels = read_grayscale_pixels(n_runs);    
    } else {
        fprintf(stderr,"Image not grayscale, extracting regular pixels\n"); 
        comp_pixels = read_byte_pixels(n_runs);
    }
    
    // Decode the run-length encoding to get full pixel array
    struct rgb *pixels = pixels_new(num_pixels);

    int j = 0; // index through compressed pixel array
    int curr_run = 0; // current run number to keep track of run encoding
    for (int i = 0; i < num_pixels; i++) {
        pixels[i] = comp_pixels[j];
        
        // Check if need to move on to next run
        curr_run++;
        if (curr_run >= run_lengths[j]) {
            j++;
            curr_run = 0;
        }
    }
    
    // Free arrays no longer needed
    free(comp_pixels);
    free(run_lengths);
    return img_new(w, h, pixels); 
}


int main(int argc, char *argv[])
{
    fprintf(stderr,"Converting PZ image to P3 image\n");

    // Read in pz from stdin, save as image struct, then write image 
    // struct to p3 file to stdout.
    struct image *img = read_pz();        
    img_show(stderr,img);
    write_p3(img);

    // Free struct
    img_free(img);
    return 0;
}
