// CMSC 15200 Winter 2022
// Project 1 reference implementation
// Adam Shaw

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "project1.h"

// P36 reference implementation

int main(int argc, char *argv[])
{
  char c1 = getchar();
  char c2 = getchar();
  if (c1!='P' || c2!='3') {
    fprintf(stderr,"%s: expected P3 file\n",argv[0]);
    exit(1);
  }

  unsigned int w;
  unsigned int h;
  unsigned int d;
  scanf("%u",&w);
  scanf("%u",&h);
  scanf("%u",&d);
  if (d!=255) {
    fprintf(stderr,"expected depth 255, setting to 255 anyway\n");
    d=255;
  }

  printf("P6\n%u %u\n%u\n",w,h,d);
  unsigned char byte;
  for (int i=0; i<w*h*3; i++) {
    scanf("%hhu",&byte);
    putchar(byte);
  }
    
  return 0;
}
