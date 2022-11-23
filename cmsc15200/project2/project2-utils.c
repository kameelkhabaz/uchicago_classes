#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "project2.h"

// compare two rgb structs for logical equality
int rgb_eq(struct rgb c1, struct rgb c2)
{
    return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b);
}

// check if image is all gray pixels
int img_grayscale(struct image *img)
{
    fprintf(stderr,"Checking if image is all grayscale.\n");
    int len = img->w * img->h;
    for (int i = 0; i < len; i++) {  
        if (!rgb_gray(img->pixels[i])) {
            fprintf(stderr,"Image is not grayscale\n");
            return 0;
        } 
    }

    fprintf(stderr,"Image is grayscale\n");
    return 1;
}

// check if rgb is grayscale            
int rgb_gray(struct rgb c)
{
    return (c.r == c.g) && (c.r == c.b);
}

// concatenate 2 strings into a 3rd string that is heap-alocated
char *concatenate(char *str1, char *str2)
{
    /* This function allocates a new string to hold both input strings */
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    char *str = (char *)malloc(len1 + len2 + 1);
    if (str == NULL) {
        fprintf(stderr,"concatenate: malloc failed\n");
        exit(1);
    }
    
    for (int i = 0; i < len1; i++) 
        str[i] = str1[i];
    
    for (int i = 0; i < len2; i++)
        str[i + len1] = str2[i];
    
    str[len1 + len2] = '\0';
    
    return str;
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
void print_lowest_2_bytes(FILE *f, int a)
{   
    putc(a >> 8 & 255, f);
    putc(a & 255, f);
}

// Read in 2 bytes as a big-endian integer
uint16_t read_big_end_2_bytes(FILE *f)
{
    unsigned char b1 = getc(f);
    unsigned char b2 = getc(f);
    return (b1 << 8) + b2;
}

// Read in 4 bytes as a big-endian integer
uint32_t read_big_end_int(FILE *f)
{
    unsigned char b1 = getc(f);
    unsigned char b2 = getc(f);
    unsigned char b3 = getc(f);
    unsigned char b4 = getc(f);
    return (b1 << 24) + (b2 << 16) + (b3 << 8) + b4;
}

// Read in string byte by byte,return heap-allocated copy 
char *read_string(FILE *f)
{
    // Keep reading until reach null terminator (999 chars max)
    char string_read[1000] = {0};
    int i = 0;
    while ((string_read[i] = getc(f)) != '\0') 
        i++;

    return strdup(string_read);
}

// Write integer in 4 bytes as big-endian integer
void write_big_end_int(FILE *f, uint32_t a)
{
    putc(a >> 24 & 255, f);
    putc(a >> 16 & 255, f);
    putc(a >> 8 & 255, f);
    putc(a & 255, f);
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

// Read in unused bytes
void read_unused_bytes(FILE *f, unsigned int n_start, unsigned int n_end)
{
    // Read from n_start to n_end inclusive
    for (int i = n_start; i <= n_end; i++) {
        getc(f);
    }
}

// Read in magic number and check if it the correct file type
void read_magic_number(FILE *f, char *correct_magic_num)
{
    char magic_num[3] = {0};
    magic_num[0] = getc(f);
    magic_num[1] = getc(f);

    // Check that file is correct file type 
    if (strcmp(magic_num,correct_magic_num)) {
        fprintf(stderr,"read_magic_number: wrong magic number/file type: "
                "expecting %s, got %s.\n",correct_magic_num,magic_num);
        exit(1);
    }
}

// Read in P3/P6 header and get w and h
void read_p3_p6_header(FILE *f, uint32_t *w, uint32_t *h, char *filetype)
{
    read_magic_number(f, filetype);

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
struct image *read_p3(FILE *f)
{
    fprintf(stderr,"Starting reading P3 image\n");

    uint32_t w, h;
    read_p3_p6_header(f, &w, &h, "P3");
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

        i++;
    }
    
    return pixels;
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

// Read in a PZ file
struct image *read_pz(char **out_description)
{
    /* Read in a pz file. There is an out-parameter to save the 
     * description. If out_description is NULL, don't save description */ 
    fprintf(stderr,"Starting reading in a PZ file\n");
    
    // Read in and check file type
    read_magic_number(stdin, "PZ");
    
    // Bytes 2-9 won't be used
    read_unused_bytes(stdin, 2, 9);

    // Get width and height of image
    uint32_t w = read_big_end_2_bytes(stdin);
    uint32_t h = read_big_end_2_bytes(stdin);

    fprintf(stderr,"Image is %d x %d\n",w,h);

    // Bytes 14-17 won't be used
    read_unused_bytes(stdin, 14, 17);

    unsigned char b18 = getc(stdin);
    // Check if image is grayscale
    unsigned char grayscale = b18 & 2;

    // Bytes 19-21 won't be used
    read_unused_bytes(stdin, 19, 21);

    //Bytes 22-?? (terminated by '\0') are description
    char *description = read_string(stdin);
    fprintf(stderr,"Description is %s\n",description);

    // Save description if out parameter is not NULL, else free it
    if (out_description != NULL)
        *out_description = description;
    else 
        free(description);    

    // Read # run lengths
    uint32_t n_runs = read_big_end_int(stdin);
    fprintf(stderr,"Image has %d runs\n",n_runs);
    
    // Get run lengths 
    uint32_t *run_lengths = arr_new(n_runs);
        
    for (int i = 0; i < n_runs; i++) {
        // Read integer to the pointer of current array element
        run_lengths[i] = read_big_end_int(stdin);
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

    int compressed_i = 0; // index through compressed pixel array
    int curr_run = 0; // current run number to keep track of run encoding
    for (int i = 0; i < num_pixels; i++) {
        pixels[i] = comp_pixels[compressed_i];
        
        // Check if need to move on to next run
        curr_run++;
        if (curr_run >= run_lengths[compressed_i]) {
            compressed_i++;
            curr_run = 0;
        }
    }
    
    // Free arrays no longer needed
    free(comp_pixels);
    free(run_lengths);
    return img_new(w, h, pixels); 
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
    print_lowest_2_bytes(stdout, time);

    // Bytes 6:9 are current date
    int year = tm->tm_year + 1900;
    int month = tm->tm_mon + 1;
    uint32_t date = year * 10000 + month * 100 + tm->tm_mday;
    fprintf(stderr,"Current date is %d:%d:%d, %d\n",year,
            month,tm->tm_mday,date);
    write_big_end_int(stdout, date);
 
    // Bytes 10:11 are image width, width always less than 2 bytes
    print_lowest_2_bytes(stdout, img->w);
    
    // Bytes 12:13 are image height, height always less than 2 bytes
    print_lowest_2_bytes(stdout, img->h);

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

    // Write number of run lengths
    rl_enc_image *rl_img = get_rl_encoded_image(img);
    write_big_end_int(stdout, rl_img->nruns);
   
    // Write the runs
    ilist *curr_run_length = rl_img->run_lengths;
    while (curr_run_length != NULL) {
        write_big_end_int(stdout, curr_run_length->first);
        curr_run_length = curr_run_length->rest;
    }
   
    // Write the pixels
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

