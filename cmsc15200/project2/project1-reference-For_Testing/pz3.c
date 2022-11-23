// CMSC 15200 Winter 2022
// Project 1 reference implementation
// Adam Shaw

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

void ignore(unsigned int n)
{
  // pull n chars out of stdin, disregard them
  for (int i=0; i<n; i++)
    getchar();
}

int main(int argc, char *argv[])
{
  char c = getchar();
  char d = getchar();
  if (c!='P' || d!='Z') {
    fprintf(stderr, "%s: expected PZ file\n", argv[0]);
    exit(1);
  }

  ignore(2); // future expansion bytes

  // uint16_t hhmm = read2(stdin);
  // uint32_t yyyymmdd = read4(stdin);
  ignore(6); // because we're not doing anything with date & time, ignore
  
  uint16_t w = read2(stdin);
  uint16_t h = read2(stdin);

  ignore(4);

  // byte 18 (grayscale and rle bits)
  uint8_t byte18 = getchar();
  int gray = byte18 & 0X02;
  int rle  = byte18 & 0X01;
  if (!rle) 
    fprintf(stderr,"warning: rle bit not set\n");

  ignore(3);

  char *msg = read_string(stdin);
  free(msg); // nothing to do with the description in pz3
  
  uint32_t num_runs = read4(stdin);

  uint32_t *runs = (uint32_t*)malloc(num_runs*4);
  struct rgb *colors = (struct rgb*)malloc(num_runs*sizeof(struct rgb));
  if (!runs || !colors) {
    fprintf(stderr, "%s: malloc failed (runs or colors)\n", argv[0]);
    exit(1);
  }
  
  for (int i=0; i<num_runs; i++)
    runs[i] = read4(stdin);

  if (!gray)
    for (int i=0; i<num_runs; i++)
      colors[i] = read3(stdin);
  else
    for (int i=0; i<num_runs; i++) {
      uint8_t g = getchar();
      colors[i].r = colors[i].g = colors[i].b = g;
    }
  
  // finally: write out the P3 file
  fprintf(stdout,"P3\n%u %u\n255\n",w,h);
  for (int i=0; i<num_runs; i++) {
    for (int j=0; j<runs[i]; j++) {
      struct rgb color = colors[i];      
      fprintf(stdout, "%u %u %u\n", color.r, color.g, color.b);
    }
  }
  
  // clean up
  free(runs);
  free(colors);
  
  return 0;
}
