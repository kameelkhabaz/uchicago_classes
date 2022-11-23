#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "project2.h"

typedef struct metadata metadata;

struct metadata {
    char *name; // name of image
    uint32_t date;
    uint16_t time;
    uint16_t w; // width
    uint16_t h; // height
    int g; // grayscale
    int rle; // run length encoding
    char *des; // description
    uint32_t runs; // number of runs 
    unsigned int long run_bytes;
    unsigned int long pixel_bytes;
};


// Create new metadata struct 
metadata *metadata_new(char *name, uint32_t date, uint16_t time, uint16_t w, 
                       uint16_t h, int g, int rle, char *des, uint32_t runs, 
                       unsigned int long run_bytes, 
                       unsigned int long pixel_bytes)
{
    metadata *meta = (metadata *)malloc(sizeof(metadata));
    if (meta == NULL) {
        fprintf(stderr,"metadata_new: allocation failed\n");
        exit(1);
    }
    // Deep copy construction - creates copies of char *'s 
    meta->name = strdup(name);
    meta->date = date;
    meta->time = time;
    meta->w = w;
    meta->h = h;
    meta->g = g;
    meta->rle = rle;
    meta->des = strdup(des);
    meta->runs = runs;
    meta->run_bytes = run_bytes;
    meta->pixel_bytes = pixel_bytes;
    
    return meta;
}

// Free metadata
void metadata_free(metadata *meta)
{
    free(meta->name);
    free(meta->des);
    free(meta);
}

int find_last_char(char *s, char v)
{
    /* Find last instance of a char in a string and return position.
     * If char is not in string or string is NULL, return -1 */

    if (s == NULL) 
        return -1;
    
    int pos = -1;
    int len = strlen(s);
    for (int i = 0; i < len; i++) {
        if (s[i] == v) 
            pos = i;
    }
    
    return pos;
}

// Read in PZ medadata
metadata *read_medadata(char *file)
{
    fprintf(stderr,"Starting reading medadata of a pz file\n");
    
    // Get last slash in argv1 to get file name
    int last_slash = find_last_char(file,'/');
    char *file_name = file + last_slash + 1;
    fprintf(stderr,"Last slash at position %d, file name is %s\n", 
            last_slash, file_name);    

    FILE *fp = fopen(file, "r");

    // Read in and check file type
    read_magic_number(fp,"PZ");
    
    // Bytes 2-3 won't be used
    read_unused_bytes(fp, 2, 3);
    
    // Bytes 4-5: Get time and date
    uint16_t time = read_big_end_2_bytes(fp);
    
    // Bytes 6-9: Get date
    uint32_t date = read_big_end_int(fp);

    // Bytes 10-13: Get width and height of image
    uint16_t w = read_big_end_2_bytes(fp);

    uint16_t h = read_big_end_2_bytes(fp); 

    fprintf(stderr,"Image is %d x %d\n",w,h);

    // Bytes 14-17 won't be used
    read_unused_bytes(fp, 14, 17);

    unsigned char b18 = getc(fp);
    // Check if image is grayscale
    unsigned char g = b18 & 2;
    int rle = b18 & 1;

    // Bytes 19-21 won't be used
    read_unused_bytes(fp, 19, 21);

    //Bytes 22-?? (terminated by '\0') are description
    char *des = read_string(fp);
    fprintf(stderr,"Image description: %s\n",des);

    // Read # run lengths
    uint32_t runs = read_big_end_int(fp);
    fprintf(stderr,"%d runs\n", runs);
    
    fclose(fp);

    unsigned int long run_bytes = runs * 4;
    unsigned int long pixel_bytes;

    if (g) {
        pixel_bytes = runs;
    } else {
        pixel_bytes = runs * 3;
    }
    
    metadata *meta = metadata_new(file_name, date, time, w, h, g, rle, 
                                  des, runs, run_bytes, pixel_bytes);
    free(des);
    return meta;
}

void create_json(metadata *meta)
{
    // Create JSON file
    printf("{\n");
    printf("  \"filename\" : \"%s\",\n", meta->name);
    printf("  \"date\" : %d,\n", meta->date);
    printf("  \"time\" : %d,\n", meta->time);
    printf("  \"width\" : %d,\n", meta->w); 
    printf("  \"height\" : %d,\n", meta->h); 
    if (meta->g) {
        printf("  \"grayscale\" : true,\n"); 
    } else {
        printf("  \"grayscale\" : false,\n"); 
    }
    
    if (meta->rle) {
        printf("  \"rle\" : true,\n");
    } else {
        printf("  \"rle\" : false,\n");
    }
    
    printf("  \"description\" : \"%s\",\n", meta->des);
    printf("  \"runs\" : %d,\n", meta->runs);
    printf("  \"run-bytes\" : %lu,\n", meta->run_bytes);
    printf("  \"pixel-bytes\" : %lu\n", meta->pixel_bytes);
    printf("}\n");
}

int main(int argc, char *argv[]) {
    // Extract metadata from pz file and put in JSON
    fprintf(stderr,"Starting PZM\n");

    if (argc <= 1) {
        fprintf(stderr,"pzm error: argv[1] is not supplied\n");
        exit(1);
    }

    // Read in metadata from pz file
    metadata *meta = read_medadata(argv[1]);

    // Write json file
    create_json(meta);
    
    // Free metadata    
    metadata_free(meta);    

    return 0;
}

