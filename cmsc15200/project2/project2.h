#ifndef _PROJECT2_H_
#define _PROJECT2_H_
#include <stdint.h>

struct rgb {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct image {
    uint32_t w;
    uint32_t h;
    struct rgb *pixels; // array of length w*h
};

// Defining list structures
// Convention: Empty list is NULL
typedef struct ilist ilist;

struct ilist {
    uint32_t first;
    ilist *rest;
};

// Convention: Empty list is NULL
typedef struct rgblist rgblist;

struct rgblist {
    struct rgb first;
    rgblist *rest;
};

// Struct for storing run length encoded image
struct rl_encoded_image {
    uint32_t w;
    uint32_t h;
    uint32_t nruns; // number of runs
    ilist *run_lengths; // linked integer list of length n_runs
    rgblist *unique_pixels; // linked struct rgb list of length n_runs
};

typedef struct rl_encoded_image rl_enc_image;
    
// an image's pixels are stored in "row order",
// with the top row first from left to right,
// then the next row underneath, etc.

/* implement these two utilities in project2-utils.c */

// compare two rgb structs for logical equality
int rgb_eq(struct rgb c1, struct rgb c2);

// check if image is all gray pixels
int img_grayscale(struct image *img);

/* you may add more utilities if you like, but don't change these two */

// concatenate two strings into a third string that is heap-allocated
char *concatenate(char *str1, char *str2);

// return heap-allocated string for rgb
char *rgb_tos(struct rgb c);

// check if rgb is grayscale
int rgb_gray(struct rgb c);

// get sum of array of 4-byte integers
int get_sum(uint32_t *arr, int len);

// print lowest 2 bytes of int
void print_lowest_2_bytes(FILE *f, int a);

// read in 2 bytes as a big-endian integer
uint16_t read_big_end_2_bytes(FILE *f);

// read in 4 bytes as a big-endian integer
uint32_t read_big_end_int(FILE *f);

// Read in string byte by byte,return heap-allocated copy 
char *read_string(FILE *f);

// Write integer in 4 bytes as big-endian integer
void write_big_end_int(FILE *f, uint32_t a);

// Create a heap-allocated ilist
ilist *ilist_new(uint32_t first, ilist *rest);

// Show integer list
void ilist_show(FILE *f, ilist *lst);

// Create a heap-allocated rgblist
rgblist *rgblist_new(struct rgb first, rgblist *rest);

// Free the ilist 
void ilist_free(ilist *lst);

// Free the rgblist
void rgblist_free(rgblist *lst);

// Create heap-allocated struct rgb
struct rgb *rgb_new(unsigned char r, unsigned char g, unsigned char b);

// Create heap-allocated array of struct rgbs
struct rgb *pixels_new(uint32_t num_pixels);

// Create a heap-allocated image struct
struct image *img_new(uint32_t w, uint32_t h, struct rgb *pixels);   

// free the image struct
void img_free(struct image *img);

// print out image
void img_show(FILE *f, struct image *img);

// Free rl_encoded_image struct
void rl_image_free(rl_enc_image *rl_img);

// Create a heap-allocated rl_encoded_image struct  
rl_enc_image *rl_image_new(uint32_t w, uint32_t h, uint32_t nruns, 
                           ilist *run_lengths, rgblist *unique_pixels);

// Convert image to run-length encoded image 
rl_enc_image *get_rl_encoded_image(struct image *img);

/* Image reading/writing helper functions */

// Read in unused bytes
void read_unused_bytes(FILE *f, unsigned int n_start, unsigned int n_end);

// Read in magic number and check if it the correct file type                   
void read_magic_number(FILE *f, char *correct_magic_number); 

// Read p3, p6 header and get w and h
void read_p3_p6_header(FILE *f, uint32_t *w, uint32_t *h, char *filetype);

// Read in a P3 file 
struct image *read_p3();

// Read in pixels byte by byte
struct rgb *read_byte_pixels(uint32_t n_pixels);

// Read in grayscale pixels byte by byte
struct rgb *read_grayscale_pixels(uint32_t n_pixels);

// Write out a P3 file
void write_p3(struct image *img);

// Create heap-allocated array of 4 byte ints
uint32_t *arr_new(uint32_t len);

// Read in a PZ file
struct image *read_pz();

// Write to a PZ file
void write_pz(struct image *img, char *description);

#endif
