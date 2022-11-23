#ifndef _PROJECT1_H_
#define _PROJECT1_H_

#include <stdint.h>
#include <stdio.h>

// ====== data definitions

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

// an image's pixels are stored in "row order",
// with the top row first from left to right,
// then the next row underneath, etc.

struct run_list_node {
  uint32_t len;
  struct rgb color;
  struct run_list_node *rest;
};

typedef struct run_list_node rlist;

// ====== operations

// --- utilities for reading bytes from file
uint16_t read2(FILE *f);
struct rgb read3(FILE *f);
uint32_t read4(FILE *f);
char *read_string(FILE *f);

// --- utilities for writing bytes to file
void fwrite4(FILE *f, uint32_t n);
void fwrite3(FILE *f, struct rgb color);
void fwrite2(FILE *f, uint16_t n);

// --- utilities for getting date and time
//     (note: this uses two out parameters)
void date_and_time(uint32_t *d, uint16_t *t);

// --- read P3 data into an image struct
struct image *image_from_P3(FILE *f);

// --- compare two rgb structs for logical equality
int rgb_eq(struct rgb c1, struct rgb c2);

// --- check if image is all gray pixels
int img_grayscale(struct image *img);

// --- build a list of runs, for run-length encoding
//     (see rlist definition above)
rlist *build_rlist(struct image *img);

// --- write run-length encoding, full color, PZ-style
//     (number of runs, then runs, then colors)
void rlist_write_PZ(FILE *f, rlist *runs);

// --- write run-length encoding, grayscale, PZ-style
//     (number of runs, then runs, then gray bytes)
void rlist_write_PZ_gray(FILE *f, rlist *runs);

// --- cleaner-uppers
void image_free(struct image *img);
void rlist_free(rlist *runs);

#endif
