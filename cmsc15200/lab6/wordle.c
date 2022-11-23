#include <stdio.h>
#include "strlist.h"

// helper functions

int start_b(char *s) 
{
    if (*s == 'B')
        return 1;
    return 0;
}

int main()
{
    printf("Testing wordle.c\n");
    FILE *f = fopen("words.txt", "r"); 
    strlist *wordle_answers = strlist_from_file(f);
    strlist_write(stdout,wordle_answers);
    printf("We found that %d Wordle answers start with the letter '%c'\n", 
           strlist_count(&start_b,wordle_answers),'B');
    
    // answer some questions about wordle_answers
}
