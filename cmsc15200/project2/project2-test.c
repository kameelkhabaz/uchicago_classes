#include <stdio.h>
#include <stdlib.h>
#include "project2.h"

/* Kameel Khabaz
 * CMSC 15200 Project 1 General Testing Functions */

int main(int argv, char *argc[])
{
    fprintf(stderr,"Starting testing helper functions.\n");
    struct rgb *c1 = rgb_new(255,0,255);
    struct rgb *c2 = rgb_new(0,2,5);
    struct rgb *c3 = rgb_new(255,0,255);
    int eq1 = rgb_eq(*c1,*c2);
    int eq2 = rgb_eq(*c1,*c3);
    printf("Color 1 is %s\n",rgb_tos(*c1));
    printf("Color 2 is %s\n",rgb_tos(*c2));
    printf("Color 3 is %s\n",rgb_tos(*c3));
    printf("(expecting 0) colors 1 and 2 are equal: %d\n",eq1);
    printf("(expecting 1) colors 1 and 3 are equal: %d\n",eq2);
    free(c1);
    free(c2);
    free(c3);
    uint32_t arr[5] = {1,2,3,4,5};
    printf("Sum of {1,2,3,4,5} is: expect 15, got %d\n", get_sum(arr,5));
    ilist *lst = ilist_new(3, ilist_new(2, ilist_new(9, NULL)));
    printf("Making int list {3,2,9}\n");
    ilist_show(stdout,lst);
    ilist_free(lst);
    return 0;
}
