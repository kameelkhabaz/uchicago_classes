// Kameel Khabaz
// CMSC Project 3
#define  _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "cachelab.h"
#include <string.h>

// Data structures for the cache 
typedef struct line line;

struct line {
    //implement linked list for lines
    char valid;
    long tag; 
    line *rest;
};


struct set {
    int E;
    line *line_list;
    // front of list has MOST recently used
    // back of list has LEAST recently used
};

typedef struct set set;

struct cache {
    int s;
    int E;
    int b;
    set **sets;
};

typedef struct cache cache; 

// Functions for the cache 
void print_line_list(line *lst) {
    while (lst != NULL) {
        printf("%d, 0x%lX: ",lst->valid, lst->tag);
        lst = lst->rest;
    }
    printf("nil\n");
}

line *initialize_line(int valid, long tag) {
    /* Initialize a line */
    line *ml = (line *)malloc(sizeof(line));
    ml->valid = valid;
    ml->tag = tag;
    ml->rest = NULL;
    return ml;
}

set *initialize_set(int E) {
    /* Initialize a set */
    set *ms = (set *)malloc(sizeof(set));
    ms->E = E;
    ms->line_list = NULL;
    return ms;
}

cache *initialize_cache(int s,int E,int b) {
    /* Initializes a cache according to parameters s, E, b */
    cache *mc = (cache *)malloc(sizeof(cache));
    mc->s = s;
    mc->E = E;
    mc->b = b;
    int S = 1 << s;
    set **sets = (set **)malloc(S * sizeof(set *));
    for (int i = 0; i < S; i++) {
        sets[i] = initialize_set(E);
    }
    mc->sets = sets;
    return mc;
}

void line_free(line *ml) {
    line *curr = ml;
    while (ml != NULL) {
        curr = ml;
        ml = ml->rest;
        free(curr); 
    }
}
 
void set_free(set *ms) {
    if (ms == NULL) {
        return;
    }
    line_free(ms->line_list);
    free(ms);
}

void cache_free(cache *mc) {
    if (mc == NULL) {
        return;
    }
    int S = 1 << (mc->s);
    for (int i = 0; i < S; i++) {
        set_free(mc->sets[i]);
    }
    free(mc->sets);
    free(mc);
}

void parse_addr(long addr, long *set, long *tag, int s, int b) {
    int mask = (1 << s) - 1;
    *set = (addr >> b) & mask;
    *tag = addr >> (b + s);
    //printf("Address 0x%lX parsed into set 0x%lX and tag 0x%lX\n",addr,*set,*tag);
}

int load_cache(cache *my_cache, long set, long tag) {
    /* Read block from memory to cache. */
    line *line_list = my_cache->sets[set]->line_list;
    line *curr = line_list;
    line *sec_to_last = NULL;
    line *prev = NULL;
    int E = my_cache->sets[set]->E;
    // see if set contains the block
    int i = 0;
    while (curr != NULL) {
        if (curr->rest != NULL && curr->rest->rest == NULL) {
            sec_to_last = curr;
        }
        if (curr->valid == 1 && curr->tag == tag) {
            // cache hit, return 1 for hit
            // must update least recently used if E > 1
            if (prev) {
                prev->rest = curr->rest;
                curr->rest = line_list;
                my_cache->sets[set]->line_list = curr;
            }
            return 1;
        }
        prev = curr;
        curr = curr->rest;
        i++;
    }
    // cache miss, return 0 for miss
    
    // create new line and add to front of list
    line *new_line = initialize_line(1, tag);
    new_line->rest = line_list;
    my_cache->sets[set]->line_list = new_line;

    // if conflict or capacity miss (if #lines == E), must remove last line
    if (i == E) {
        // create new line, add to front, remove end of list
        if (sec_to_last == NULL) {
            // if only 1 elem in list and E = 1
            line_free(new_line->rest);
            new_line->rest = NULL;
        } else {
            // if E elems in list for E > 1
            line_free(sec_to_last->rest);
            sec_to_last->rest = NULL;
        }
        return 2; // return 2 for eviction
    } else {
        // no eviction, return 0
        return 0;
    }
}


int store_cache(cache *my_cache, long set, long tag) {
    // same implementation for load and store
    return load_cache(my_cache, set, tag);   
}

char *print_incr_counter(int r, int *hits, int *misses, int *evictions,
            char opr) {
    /* print if verbose, r is return val from cache op */
    switch(r) {
        case 1:
            (*hits)++;
            return strdup("hit");
        case 2:
            (*evictions)++;
            (*misses)++;
            return strdup("miss eviction");
        case 0:
            (*misses)++;
            return strdup("miss");
        default:
            fprintf(stderr,"Invalid cache op return %d\n",r);
            exit(1);
   }
}

char *strcat_heap(char *str1, char *str2) {
    char buff[256] = {0};
    strcat(buff, str1);
    strcat(buff, " ");
    strcat(buff, str2);
    return strdup(buff);
}

void cache_operation(cache *my_cache, char opr, long addr, int *hits, 
                        int *misses, int *evictions, int verbose, int size) {
    long set, tag;
    parse_addr(addr,&set, &tag, my_cache->s, my_cache->b);
    //printf("Performing instruction to %c set 0x%lX and tag 0x%lX\n",opr,set, tag);
    int r;
    char *str; // contains string descriptor of result for verbose print 
    switch (opr) {
        case 'L':
            r = load_cache(my_cache, set, tag);
            str = print_incr_counter(r, hits, misses, evictions, opr);
            break;
        case 'S':
            r = store_cache(my_cache, set, tag);
            str = print_incr_counter(r, hits, misses, evictions, opr);
            break;
        case 'M':
            r = load_cache(my_cache, set, tag);   
            char *str1 = print_incr_counter(r, hits, misses, evictions, opr);
            r = store_cache(my_cache, set, tag); 
            char *str2 = print_incr_counter(r, hits, misses, evictions, opr);
            str = strcat_heap(str1, str2);
            free(str1); free(str2);
            break;
        default:
            fprintf(stderr,"Invalid cache operation: %c\n",opr);
            exit(1);
    }
    if (verbose) {
        //printf("%c %lx,%d %s\n",opr,addr,size,str);
        //printf("set %ld list: ",set);
        print_line_list(my_cache->sets[set]->line_list);
    }
    free(str);
}

int main(int argc, char *argv[])
{
    /* v: boolean for verbose flag
     * s: number of set index bits
     * E: associativity 
     * b: number of block bits
     * t: trace file name
     * m: total number of bits in address
     */
    // Parse the arguments and initialize the cache 
    //int m = 48;
    int s = 0; int E = 0; int b = 0; int i = 0; int v = 0;
    char t[128] = {0};
    while (i < argc) {
        switch (argv[i][1]) {
            case 's':
                s = atoi(argv[++i]);
                break;  
            case 'E':
                E = atoi(argv[++i]);
                break;
            case 'b':
                b = atoi(argv[++i]);
                break;
            case 't':
                sprintf(t,"%s",argv[++i]);
                break;
            case 'v':
                v = 1;
        }
        i++;
    }
    //printf("v: %d, s: %d, E: %d, b: %d, t: %s\n",v,s,E,b,t);
    cache *my_cache = initialize_cache(s,E,b);
    
    // Start parsing the instruction file 

    FILE *fp = fopen(t, "r");
    if (fp == NULL) {
        fprintf(stderr,"Invalid trace file %s\n",t);
        exit(1);
    }

    int hits = 0; int misses = 0; int evictions = 0;

    char *line = NULL;
    size_t len = 0;
    size_t chars;
    while ((chars = getline(&line, &len, fp)) != -1) {
        char opr; long addr; int size;
        sscanf(line, " %c %lX, %d \n",&opr, &addr, &size);
        if (opr != 'I') {
            cache_operation(my_cache, opr, addr, &hits, &misses, &evictions, 
                        v, size);
        }
    }

    free(line);
    fclose(fp);
    cache_free(my_cache);    
    printSummary(hits, misses, evictions);
    return 0;
}

