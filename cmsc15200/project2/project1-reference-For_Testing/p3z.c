// CMSC 15200 Winter 2022
// Project 1 reference implementation
// Adam Shaw

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "project1.h"

void future_expansion(FILE *f, int num_bytes)
{
  for (int i=0; i<num_bytes; i++)
    putc('.', f);
}

void write_PZ(FILE *f, struct image *img, char *descrip)
{  
  fprintf(f,"PZ");
  future_expansion(f,2);

  uint32_t d32;
  uint16_t t16;
  date_and_time(&d32,&t16);
  fwrite2(f,t16);
  fwrite4(f,d32);

  fwrite2(f,img->w);
  fwrite2(f,img->h);
  future_expansion(f,4);

  int gray = img_grayscale(img);
  putc(gray?0X03:0X01,f);
  future_expansion(f,3);

  fprintf(f,"%s",descrip);
  putc('\0',f);

  rlist *runs = build_rlist(img);
  if (gray)
    rlist_write_PZ_gray(f,runs);
  else
    rlist_write_PZ(f,runs);
  rlist_free(runs);
} 

int main(int argc, char *argv[])
{
  struct image *img = image_from_P3(stdin);
  write_PZ(stdout, img, argc<2?"[image]":argv[1]);  
  image_free(img);  
  return 0;
}
