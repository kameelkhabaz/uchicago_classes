#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "project1.h"

// compare two rgb structs for logical equality
int rgb_eq(struct rgb c1, struct rgb c2)
{
    char *s1 = rgb_tos(c1);
    char *s2 = rgb_tos(c2);
    fprintf(stderr,"Checking equality of %s and %s.\n",s1,s2);
    free(s1);
    free(s2);
    return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b);
}

// check if image is all gray pixels
int img_grayscale(struct image *img)
{
    fprintf(stderr,"Checking if image is all grayscale.\n");
    int len = img->w * img->h;
    for (int i = 0; i < len; i++) {  
        if (!rgb_gray(img->pixels[i])) {
            return 0;
        } 
    }
    // Image must be all gray pixels
    return 1;
}

// check if rgb is grayscale            
int rgb_gray(struct rgb c)
{
    char *s = rgb_tos(c);
    fprintf(stderr,"Checking if %s is grayscale.\n",s);
    free(s);
    return (c.r == c.g) && (c.r == c.b);
}

// return heap-allocated string for rgb                  
char *rgb_tos(struct rgb c)
{
    char rgb_s[128] = {0};
    sprintf(rgb_s,"(%d,%d,%d)",c.r,c.g,c.b);
    return strdup(rgb_s);
}

// compute array sum of 4-byte integers
int get_sum(uint32_t *arr, int len)
{
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += arr[i];
    }
    return sum;
}

// print lowest 2 bytes of int as big-endian
void print_lowest_2_bytes(int a)
{   
    putc(a >> 8 & 255, stdout);
    putc(a & 255, stdout);
}

// Read in 4 bytes as a big-endian integer
uint32_t read_big_end_int()
{
    unsigned char b1 = getc(stdin);
    unsigned char b2 = getc(stdin);
    unsigned char b3 = getc(stdin);
    unsigned char b4 = getc(stdin);
    return (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
}

// Write integer in 4 bytes as big-endian integer
void write_big_end_int(uint32_t a)
{
    putc(a >> 24 & 255, stdout);
    putc(a >> 16 & 255, stdout);
    putc(a >> 8 & 255, stdout);
    putc(a & 255, stdout);
}

// Create a heap-allocated ilist
ilist *ilist_new(uint32_t first, ilist *rest)
{
    ilist *lst = (ilist *)malloc(sizeof(ilist));
    if (lst == NULL) {
        fprintf(stderr,"ilist_new: allocation failed\n");
        exit(1);
    }
    lst->first = first;
    lst->rest = rest;
    return lst;
}

// Show integer list
void ilist_show(FILE *f, ilist *lst)
{
    while (lst != NULL) {
        fprintf(f,"%d: ",lst->first);
        lst = lst->rest;
    }
    fprintf(f,"nil\n");
}

// Create a heap-allocated rgblist
rgblist *rgblist_new(struct rgb first, rgblist *rest)
{
    rgblist *lst = (rgblist *)malloc(sizeof(rgblist));
    if (lst == NULL) {
        fprintf(stderr,"rgblist_new: allocation failed\n");
        exit(1);
    }
    lst->first = first;
    lst->rest = rest;
    return lst;
}
 
// Free the ilist 
void ilist_free(ilist *lst)
{
    ilist *curr = lst;
    while (lst != NULL) {
        curr = lst;
        lst = lst->rest;
        free(curr); 
    }
}

// Free the rgblist
void rgblist_free(rgblist *lst)
{
    rgblist *curr = lst;
    while (lst != NULL) {
        curr = lst;
        lst = lst->rest;
        free(curr); 
    }
}
 
// create a new heap-allocated rgb
struct rgb *rgb_new(unsigned char r, unsigned char g, unsigned char b)          
{                                                                               
    struct rgb *c = (struct rgb *)malloc(sizeof(struct rgb));                   
    if (c == NULL) {
        fprintf(stderr,"rgb_new: allocation error.\n");
        exit(1);
    }

    c->r = r;                                                                   
    c->g = g;                                                                   
    c->b = b;                                                                   
    return c;                                                                   
}

// create new heap-allocated array of rgb structs
struct rgb *pixels_new(uint32_t n_pixels) 
{
    struct rgb *pixels = (struct rgb *)malloc(n_pixels * sizeof(struct rgb));
    if (pixels == NULL) {
        fprintf(stderr,"pixels_new: allocation error.\n");
        exit(1);
    }
    return pixels;
}

// Create a new heap-allocated image struct
struct image *img_new(uint32_t w, uint32_t h, struct rgb *pixels)
{
    // Create image struct
    struct image *img = (struct image *)malloc(sizeof(struct image));
    if (img == NULL) {
        fprintf(stderr,"img_new: allocation error.\n");
        exit(1);
    }

    img->w = w;
    img->h = h;
    img->pixels = pixels;
    
    return img;
}

// Free image struct
void img_free(struct image *img)
{
    free(img->pixels);
    free(img);
}


// print out image
void img_show(FILE *f, struct image *img)
{
    fprintf(f,"Printing out image size %d x %d:\n",img->w,img->h);
    for (int i = 0; i < img->w * img->h; i++) {
        char *rgbstr = rgb_tos(img->pixels[i]);
        fprintf(f,"%s\n",rgbstr);
        free(rgbstr);
    }
    fprintf(f,"Finished printing image\n");
}


// Read in magic number and check if it the correct file type
void read_magic_number(char *correct_magic_num)
{
    char magic_num[3] = {0};
    magic_num[0] = getc(stdin);
    magic_num[1] = getc(stdin);

    // Check that file is correct file type 
    if (strcmp(magic_num,correct_magic_num)) {
        fprintf(stderr,"read_magic_number: wrong magic number/file type: "
                "expecting %s, got %s.\n",correct_magic_num,magic_num);
        exit(1);
    }
}

// Read in P3/P6 header and get w and h
void read_p3_p6_header(uint32_t *w, uint32_t *h, char *filetype)
{
    read_magic_number(filetype);

    // Read in height and width
    scanf("%d %d",w,h);
    fprintf(stderr,"Image is %d x %d\n",*w,*h);

    // Read max color val
    unsigned int max_col;
    scanf("%d\n",&max_col);
    if (max_col != 255) {
        fprintf(stderr,"WARNING: Max col is not 255, it is %d\n",max_col);
        fprintf(stderr,"Program will not normalize the colors\n");
    }
}
 
// read in a P3 file
struct image *read_p3()
{
    fprintf(stderr,"Starting reading P3 image\n");

    uint32_t w, h;
    read_p3_p6_header(&w, &h, "P3");
    uint32_t n_pixels = w * h;
    // Now read in pixels
    struct rgb *pixels = pixels_new(n_pixels);

    int i = 0;
    unsigned char r,g,b;
    while (scanf("%hhu %hhu %hhu",&r,&g,&b) != EOF) {
        // Check dimensions
        if (i > n_pixels - 1) {
            fprintf(stderr,"read_p3: Incorrect image dimensions.\n");
            exit(1);
        }
        // Assign pixel
        pixels[i].r = r;
        pixels[i].g = g;
        pixels[i].b = b;
        fprintf(stderr,"Read in %d %d %d\n",pixels[i].r,
                pixels[i].g,pixels[i].b);
        i++;
    }
    
    return img_new(w, h, pixels);
}

// read in rgb pixels byte by byte
struct rgb *read_byte_pixels(uint32_t n_pixels)
{
    // Now read in pixels
    struct rgb *pixels = pixels_new(n_pixels);

    int i = 0;
    int r, g, b;
    while ((r = getc(stdin)) != EOF) {
        g = getc(stdin);
        b = getc(stdin);
        // Check dimensions
        if (i > n_pixels - 1) {
            fprintf(stderr,"read_byte_pixels: Wrong image dimensions.\n");
            exit(1);
        }
        // Assign pixel
        pixels[i].r = r;
        pixels[i].g = g;
        pixels[i].b = b;
        
        fprintf(stderr,"Read in %d %d %d for pixel %d\n",pixels[i].r,
                pixels[i].g,pixels[i].b,i);
        i++;
    }
    
    return pixels;
}


// Write to a P3 file
void write_p3(struct image *img)
{   
    fprintf(stderr,"Starting writing P3 file\n");
    // Write to stdout
    printf("P3\n");
    printf("%d %d\n",img->w, img->h);
    
    // Max color value is 255 by default
    printf("255\n");
    
    if (img->w * img->h == 0) {
        fprintf(stderr,"Image is 0x0, finished writing P3 file.\n");
        return;
    }
 
    // Write the pixels 
    for (int i = 0; i < img->w * img->h; i++) {
        printf("%d %d %d\n", img->pixels[i].r, 
                img->pixels[i].g, img->pixels[i].b);
    }
    fprintf(stderr,"Finished writing P3 file\n");
}
