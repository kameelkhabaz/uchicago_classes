#include <stdio.h>
#include <string.h>
#include "strlist.h"

int longer_than_2(char *s) 
{
    int len = strlen(s);
    return len > 2 ? 1 : 0;
}

int main()
{
    strlist *strlist1 = strlist_new("Hi", NULL);
    strlist_write(stdout,strlist1);
    printf("strlist_len(strlist1): %u\n", strlist_len(strlist1));
    printf("#Words > 2 characters: expect %d, got %d\n",0,
            strlist_count(&longer_than_2,strlist1));
    strlist_free(strlist1);
    
    FILE *f = fopen("tiny.txt","r");
    strlist *strlist2 = strlist_from_file(f);
    strlist_write(stdout,strlist2);
    printf("strlist_len(strlist2): %u\n", strlist_len(strlist2));
    printf("#Words > 2 characters: expect %d, got %d\n",2,
            strlist_count(&longer_than_2,strlist2));
    strlist_free(strlist2);
    fclose(f);
    return 0;
}
