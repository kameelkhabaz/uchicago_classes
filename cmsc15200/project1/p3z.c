#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// Struct for storing run length encoded image
struct rl_encoded_image {
    uint32_t w;
    uint32_t h;
    uint32_t nruns; // number of runs
    ilist *run_lengths; // linked integer list of length n_runs
    rgblist *unique_pixels; // linked struct rgb list of length n_runs
};

typedef struct rl_encoded_image rl_enc_image;
 
// Free rl_encoded_image struct
void rl_image_free(rl_enc_image *rl_img)
{
    ilist_free(rl_img->run_lengths);
    rgblist_free(rl_img->unique_pixels);
    free(rl_img);
}

// Create a heap-allocated rl_encoded_image struct  
rl_enc_image *rl_image_new(uint32_t w, uint32_t h, uint32_t nruns, 
                           ilist *run_lengths, rgblist *unique_pixels)
{
    rl_enc_image *rl_img = (rl_enc_image *)malloc(sizeof(rl_enc_image));

    if (rl_img == NULL) {
        fprintf(stderr, "rl_image_new: allocation error.\n");
        exit(1);
    }
    
    rl_img->w = w;
    rl_img->h = h;
    rl_img->nruns = nruns;
    rl_img->run_lengths = run_lengths;
    rl_img->unique_pixels = unique_pixels;
    
    return rl_img;
}


// Convert image to run-length encoded image 
rl_enc_image *get_rl_encoded_image(struct image *img)
{
    int n_pixels = img->w * img->h;
    // Check for 0x0 image
    if (!n_pixels) {
        return rl_image_new(0,0,0,NULL,NULL);
    }

    // Initialize lists for storing run lengths and unique pixels
    ilist *run_lengths = ilist_new(0,NULL);
    ilist *curr_run_length = run_lengths; // last node in ilist

    rgblist *unique_pixels = rgblist_new(img->pixels[0],NULL);
    rgblist *curr_unique = unique_pixels; // last node in rgblist

    // Store last unique color and its index in pixels array
    struct rgb last_unique = unique_pixels->first; 
    int last_unique_idx = 0; 

    // Number of runs 
    uint32_t nruns = 1;

    for (int i = 1; i < n_pixels; i++) {
        // Check if this pixel is different from the previous one
        if (!rgb_eq(img->pixels[i],last_unique)) {
            curr_run_length->first = i - last_unique_idx;
            // Move on to next node in linked list
            curr_unique->rest = rgblist_new(img->pixels[i],NULL);
            curr_run_length->rest = ilist_new(0,NULL);
            curr_unique = curr_unique->rest;
            curr_run_length = curr_run_length->rest;
            // Update temporary variables 
            last_unique = curr_unique->first;
            last_unique_idx = i;
            nruns++;
        }
    }   
    // Set final run length
    curr_run_length->first = n_pixels - last_unique_idx;

    fprintf(stderr,"Image has %d pixels, %d runs\n",n_pixels,nruns);
 
    return rl_image_new(img->w, img->h, nruns, run_lengths, unique_pixels); 
}

// Write to a PZ file
void write_pz(struct image *img, char *description)
{
    fprintf(stderr,"Starting writing PZ file\n");
    // Write to stdout
    printf("PZ"); // bytes 0:1
    printf(".."); // bytes 2:3 
    
    // Bytes 4:5 are current time
    time_t curr_time;
    time(&curr_time);
    struct tm *tm = localtime(&curr_time);
    int time = tm->tm_hour * 100 + tm->tm_min;
    fprintf(stderr,"Local time %d:%d, %d\n", tm->tm_hour, tm->tm_min, time);
    print_lowest_2_bytes(time);

    // Bytes 6:9 are acurrent date
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    uint32_t date = year * 10000 + month * 100 + tm->tm_mday;
    fprintf(stderr,"Current date is %d:%d:%d, %d\n",year,
            month,tm->tm_mday,date);
    write_big_end_int(date);
 
    // Bytes 10:11 are image width, width always less than 2 bytes
    print_lowest_2_bytes(img->w);
    
    // Bytes 12:13 are image height, height always less than 2 bytes
    print_lowest_2_bytes(img->h);

    // Bytes 14:17 reserved for future expansion
    for (int i = 14; i <= 17; i++)
        putc('.',stdout);
    
    int isgray = img_grayscale(img);
    
    // Byte 18
    if (isgray)
        putc(0x03,stdout);
    else 
        putc(0x01,stdout);

    // Bytes 19:21 reserved for future expansion
    for (int i = 19; i <= 21; i++)
        putc('.',stdout);
    
    
    // Bytes 22:?? description
    fprintf(stderr,"Image description is \"%s\"\n",description);
    printf("%s",description);
    putc('\0',stdout); // print null terminating stiring

    rl_enc_image *rl_img = get_rl_encoded_image(img);
    write_big_end_int(rl_img->nruns);
   
    ilist *curr_run_length = rl_img->run_lengths;
    while (curr_run_length != NULL) {
        write_big_end_int(curr_run_length->first);
        curr_run_length = curr_run_length->rest;
    }
   
    rgblist *curr_unique = rl_img->unique_pixels;
    while (curr_unique != NULL) { 
        struct rgb pixel = curr_unique->first;
        putc(pixel.r,stdout);
        // Print out g and b values if not grayscale
        if (!isgray) {
            putc(pixel.g,stdout);
            putc(pixel.b,stdout);
        }
        curr_unique = curr_unique->rest;
    }
      
    rl_image_free(rl_img);
    fprintf(stderr,"Finished writing PZ file.\n");
}


int main(int argc, char *argv[])
{
    fprintf(stderr,"Converting P3 image to PZ image\n");

    // Read in p3 from stdin, save as image struct, then write image 
    // struct to pz file to stdout.
    struct image *img = read_p3();        
    img_show(stderr,img);
  
    // Check for descriptor string
    if (argc > 1)
        write_pz(img,argv[1]);
    else
        write_pz(img,"[image, no description]");

    // Free img struct
    img_free(img);
    return 0;
}

