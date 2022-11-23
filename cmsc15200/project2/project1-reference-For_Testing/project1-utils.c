#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "project1.h"

uint16_t read2(FILE *f)
{
  unsigned char a = getc(f);
  unsigned char b = getc(f);
  return (a<<8)|b;
}

struct rgb read3(FILE *f)
{
  unsigned char a = getc(f);
  unsigned char b = getc(f);
  unsigned char c = getc(f);
  struct rgb ret = {a,b,c};
  return ret;
}

uint32_t read4(FILE *f)
{
  unsigned char a = getc(f);
  unsigned char b = getc(f);
  unsigned char c = getc(f);
  unsigned char d = getc(f);
  return (a<<24)|(b<<16)|(c<<8)|d;
}

char *read_string(FILE *f)
{
  // max 1024 chars
  char buf[1024] = {0};
  int c, i=0;
  while ((c=getc(f)))
    buf[i++]=c;
  if (i>=1024) {
    fprintf(stderr, "read_string: overflow\n");
    exit(1);
  }
  return strdup(buf);
}

void fwrite4(FILE *f, uint32_t n)
{
  putc(n>>24, f);
  putc((n>>16)&0XFF, f);
  putc((n>>8)&0XFF, f);
  putc(n&0XFF, f);
}

void fwrite3(FILE *f, struct rgb color)
{
  putc(color.r, f);
  putc(color.g, f);
  putc(color.b, f);
}

void fwrite2(FILE *f, uint16_t n)
{
  putc((n>>8)&0XFF, f);
  putc(n&0XFF, f);
}

void date_and_time(uint32_t *d, uint16_t *t)
{
  // here, d and t are "out parameters"
  // - this way I can "return" two values without
  //   needing to design a custom struct
  time_t tt = time(NULL);
  struct tm *lt = localtime(&tt);
  uint32_t packd = (lt->tm_year+1900)*10000 + (lt->tm_mon+1)*100 + lt->tm_mday;
  uint16_t packt = lt->tm_hour*100 + lt->tm_min;
  *d = packd;
  *t = packt;
}

uint16_t time_hhmm()
{
  time_t tt = time(NULL);
  struct tm *t = localtime(&tt);
  uint16_t hhmm = t->tm_hour*100 + t->tm_min;
  return hhmm;
}

uint32_t date_yyyymmdd()
{
  time_t tt = time(NULL);
  struct tm *t = localtime(&tt);
  uint32_t d = (t->tm_year+1900)*10000 + (t->tm_mon+1)*100 + t->tm_mday;
  return d;
}

struct image *image_from_P3(FILE *f)
{
  char c1 = getc(f);
  char c2 = getc(f);
  if (c1!='P' || c2!='3') {
    fprintf(stderr, "image_from_P3: expected P3\n");
    exit(1);
  }

  unsigned int w;
  unsigned int h;
  unsigned int d;
  fscanf(f,"%u",&w);
  fscanf(f,"%u",&h);  
  fscanf(f,"%u",&d);
  if (d!=255) {
    fprintf(stderr,"WARNING: expected color depth 255, ignoring %u\n",d);
    d=255;
  }

  struct rgb *px = (struct rgb *)malloc(sizeof(struct rgb)*(w*h));
  if (!px) {
    fprintf(stderr,"image_from_P3: malloc failed (px)\n");
    exit(1);
  }

  for (int i=0; i<w*h; i++) {
    unsigned char r, g, b;
    fscanf(f,"%hhu",&r);
    fscanf(f,"%hhu",&g);
    fscanf(f,"%hhu",&b);
    px[i].r = r;
    px[i].g = g;
    px[i].b = b;
  }

  struct image *img = (struct image*)malloc(sizeof(struct image));
  if (!img) {
    fprintf(stderr, "image_from_p3: malloc failed (img)\n");
    exit(1);
  }
  img->w = w;
  img->h = h;
  img->pixels = px;
  
  return img;
}

// compare two rgb structs for logical equality
int rgb_eq(struct rgb c1, struct rgb c2)
{
  return c1.r==c2.r && c1.g==c2.g && c1.b==c2.b;
}

// check if image is all gray pixels
int img_grayscale(struct image *img)
{
  uint32_t n = img->w * img->h;
  for (int i=0; i<n; i++) {
    struct rgb c = img->pixels[i];
    if (c.r==c.g && c.g==c.b)
      continue; // jump to next loop iteration
    return 0;
  }
  return 1;    
}

rlist *rlist_new(uint32_t len, struct rgb color, rlist *r)
{
  rlist *t = (rlist*)malloc(sizeof(rlist));
  if (!t) {
    fprintf(stderr,"rlist_new: malloc failed\n");
    exit(1);
  }
  t->len = len;
  t->color = color;
  t->rest = r;
  return t;
}

rlist *build_rlist(struct image *img)
{
  uint32_t n = img->w * img->h;
  if (n==0)
    return NULL;
  struct rgb last_color = img->pixels[n-1];
  rlist *runs = rlist_new(1,last_color,NULL);
  // by constructing this list from right to left ("backwards"),
  // it ends up in the correct order when done
  for (int i=n-2; i>=0; i--) {
    struct rgb curr_color = img->pixels[i];
    if (rgb_eq(last_color, curr_color)) {
      runs->len++;
    } else {
      runs = rlist_new(1,curr_color,runs);
      last_color = curr_color;
    }
  }
  return runs;
}

void rlist_write_PZ(FILE *f, rlist *runs)
{
  // write the number of runs 
  uint32_t n = 0;
  for (rlist *curr = runs; curr; curr=curr->rest) ++n;
  fwrite4(f, n);
  // write the run lengths
  for (rlist *curr = runs; curr; curr=curr->rest)
    fwrite4(f, curr->len);
  // write the colors
  for (rlist *curr = runs; curr; curr=curr->rest)
    fwrite3(f, curr->color);
}

void rlist_write_PZ_gray(FILE *f, rlist *runs)
{
  // this is the grayscale version of rlist_write_PZ
  // write the number of runs 
  uint32_t n = 0;
  for (rlist *curr = runs; curr; curr=curr->rest) ++n;
  fwrite4(f, n);
  // write the run lengths
  for (rlist *curr = runs; curr; curr=curr->rest)
    fwrite4(f, curr->len);
  // write the gray values
  for (rlist *curr = runs; curr; curr=curr->rest)
    putc(curr->color.r, f);
}

void image_free(struct image *img)
{
  if (img) {
    free(img->pixels);
    free(img);
  }
}

void rlist_free(rlist *runs)
{
  while (runs) {
    rlist *next = runs->rest;
    free(runs);
    runs = next;
  }
}
